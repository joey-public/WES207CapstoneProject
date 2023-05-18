#include "host.h"
#include <boost/asio/ip/tcp.hpp>
#include <fstream>
#include <istream>
#include "PacketUtils.h"

#include "UsrpRxStreamFuncs.h"
#include "UtilFuncs.h"
#include "ProcessingFuncs.h"

//#include <gnuplot-iostream.h>

//#define SAVE_DATA
//#define SAVE_DATA_PULSE

typedef struct md_time_data_s
{
    int64_t full_secs;
    double frac_secs;
}md_time_data;

Client::Client(boost::asio::io_context& io_context, 
        const std::string& server_address, 
        const std::string& server_port,
        const std::string& usrp_ip)
    : io_context_(io_context),
      socket_(io_context),
      server_address_(server_address),
      server_port_(server_port),
      client_id_(0),
      is_configured_(false),
      is_synchronized_(false),
      is_streaming_(false),
      usrp_ip_(usrp_ip),
      stream_seq_id(0),
      stream_pkt_id(0)
{
      
}

void Client::start()
{
    std::cout << "Connecting to server " << server_address_ << ":" << server_port_ << "..." << std::endl;
    std::cout << "USRP IP Address: " << usrp_ip_ << std::endl;
    boost::asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(server_address_, server_port_);
    //socket_.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 33334 ));
    boost::asio::connect(socket_, endpoints);
    std::cout << "Connected to server." << std::endl;

    // Get assigned client ID from server
    boost::asio::streambuf response_buffer;
    boost::asio::read_until(socket_, response_buffer, '\n');
    std::istream response_stream(&response_buffer);
    response_stream >> client_id_;
    std::cout << "Assigned client ID: " << client_id_ << std::endl;
    
    control_command_thread_ = std::thread(&Client::control_command_handler, this);
    //dsp_thread_ = std::thread(&Client::dsp_handler, this);
}

void Client::stop()
{
    std::cout << "Disconnecting from server..." << std::endl;
    socket_.close();
    std::cout << "Disconnected from server." << std::endl;
}

void Client::configure_usrp()
{
    std::cout << "Configuring USRP..." << std::endl;
      
      //usrp settings
    std::string ip          = usrp_ip_;
    std::string subdev      = "A:0";
    std::string ant         = "TX/RX";
    std::string clock_ref   = "external";
    std::string time_ref    = "external";
    double sample_rate      = 10e6;
    double center_freq      = 173935300;
    double gain             = 0;
    double bw               = 10e6;
    //stream settings
    std::string cpu_fmt     = "sc16";
    std::string wire_fmt    = "sc16";
    size_t channel          = 0;
    double setup_time       = 1;

    if(NULL == usrp_handler && false == is_configured_)
    {
        usrp_handler = new UsrpInitilizer(ip, subdev, ant, 
                                    clock_ref, time_ref,
                                    sample_rate, center_freq, gain, bw);
    }
    std::cout << boost::format("Using Device: %s") % usrp_handler->get_usrp()->get_pp_string() << std::endl;
    std::cout << usrp_handler->get_clock_ref() << std::endl;
    std::cout << usrp_handler->get_time_ref() << std::endl;
    std::cout << boost::format("Actual RX Freq: %f MHz...")
                         % (usrp_handler->get_usrp()->get_rx_freq(channel) / 1e6)
                  << std::endl
                  << std::endl;
    std::cout << boost::format("Actual RX Rate: %f Msps...")
                     % (usrp_handler->get_usrp()->get_rx_rate(channel) / 1e6)
              << std::endl
              << std::endl;
    std::cout << boost::format("Actual RX Gain: %f dB...")
                         % usrp_handler->get_usrp()->get_rx_gain(channel)
                  << std::endl
                  << std::endl;
     std::cout << boost::format("Actual RX Bandwidth: %f MHz...")
                         % (usrp_handler->get_usrp()->get_rx_bandwidth(channel) / 1e6)
                  << std::endl
                  << std::endl;
    
    //sleep for a while for config to take place
    std::this_thread::sleep_for(std::chrono::milliseconds(int64_t(1000 * setup_time)));
    
    // configure USRP here
    is_configured_ = true;
    std::cout << "USRP configured." << std::endl;
}

void Client::synchronize_pps()
{
    std::cout << "Synchronizing to PPS..." << std::endl;
    usrp_handler->get_usrp()->set_time_unknown_pps(uhd::time_spec_t(0.0));
    std::cout << std::endl << "Success!" << std::endl << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep for one sec
    
    /**Check ref lock*/
    std::cout << "Product requires verification of ref_locked sensor!" << std::endl;
    std::cout << "Checking ref_locked sensor..." << std::flush;
    if (!usrp_handler->get_usrp()->get_mboard_sensor("ref_locked").to_bool()) 
    {
        std::cout << "PPS sync FAIL!" << std::endl;
    }
    else
    {
        std::cout << "PPS sync PASS!" << std::endl;
    }
    uhd::time_spec_t time_spec = usrp_handler->get_usrp()->get_time_now();

    std::cout << "Current USRP Time frac sec: " << time_spec.get_frac_secs() << std::endl;
    std::cout << "Current USRP Time full sec: " << time_spec.get_full_secs() << std::endl;
    std::cout << "Current USRP Time Real sec: " << time_spec.get_real_secs() << std::endl;
    std::cout << "Current USRP Time Tick count: "<< time_spec.get_tick_count(time_spec.get_frac_secs()) << std::endl;

    is_synchronized_ = true;
    std::cout << "Synchronized to PPS." << std::endl;
}

void Client::start_streaming()
{
    uhd::usrp::multi_usrp::sptr usrp = usrp_handler->get_usrp(); 

    is_streaming_ = true;

    //calc buffer size for desired time
    int stream_time = 3;//seconds
    size_t buffer_sz = stream_time * usrp->get_rx_rate();

    //Stream the raw data
    //fill the buffer with data from the usrp
    std::vector<std::complex<float>> data_buffer(buffer_sz);
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Start Streaming Data..." << std::endl;
    rx_strm::stream_rx_data_nsamps(usrp, buffer_sz, &data_buffer.front());
    std::cout << "Stop Streaming Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    //Analyze the raw data
    std::cout << "Analyzing Raw Data..." << std::endl;
    float buff_mem = sizeof(std::complex<float>) * buffer_sz;//bytes 
    //print stats about size of data buffer
    std::cout << "\tCollected " << stream_time << "s of raw data at fs = "
              << usrp->get_rx_rate() << std::endl;
    std::cout << "\tBuffer length: " << buffer_sz << std::endl;
    std::cout << "\tBuffer takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file (optional)
    std::string data_file_path = ""; 
#ifdef SAVE_DATA
    std::cout << "\tSaving Raw data to txt file\n";
    data_file_path = "./raw_data.txt"; 
    util::save_complex_vec_to_file(data_buffer, data_file_path);
#endif
    std::cout << "Done Analyzing Raw Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    //Process the data
    std::cout << "Start Processing Data..." << std::endl;
    float threshold = 0.01;
    float save_time = 0.02;//20ms 
    int offset_time = 0*usrp->get_rx_rate();
    std::cout << "\tTakeing the magnitude..." << std::endl;
    std::vector<float> mag_data = proc::calc_mag(data_buffer);
    buff_mem = sizeof(float) * mag_data.size();//bytes 
    std::cout << "\tMag Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file (optional)
#ifdef SAVE_DATA
    std::cout << "\tSaving Mag data to txt file...\n";
    data_file_path = "./mag_data.txt"; 
    util::save_float_vec_to_file(mag_data, data_file_path);
#endif
    std::cout << "\tDoing threshold detection..." << std::endl;
    int start_idx = proc::detect_threshold(mag_data, threshold, offset_time);
    if (start_idx < 0)
    {
        this->peak_timestamp_.push_back(0.0); 
        std::cout << "\tNo Peak Detected...\n";
    }
    else
    {
        this->peak_timestamp_.push_back(start_idx / usrp->get_rx_rate());  
        std::cout << "\tPulse Detected starting at index: " << start_idx << std::endl;
        int k = int(save_time * usrp->get_rx_rate());
        //save the pulse into a vector
        this->waveform_samples_ = util::get_subvec(data_buffer, start_idx, k);
        buff_mem = sizeof(std::complex<float>) * this->waveform_samples_.size();//bytes 
        std::cout << "\tPulse Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
        //save data to file
#ifdef SAVE_DATA_PULSE
        std::cout << "\tSaving Pulse data to txt file\n";
        data_file_path = "./pulse_data.txt"; 
        util::save_complex_vec_to_file(this->waveform_samples_, data_file_path);
#endif
    }
    std::cout << "Stop Processing Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << "Sending Packet to Host Controller..." << std::endl;
    //send packet to host_controller
    send_dsp_data();
    
    //once streaming is done, set the condition variable, so that dsp thread for can start sending samples.
    if (is_streaming_)
    {
        std::unique_lock<std::mutex> lock(mutex_host);
        is_streaming_ = false;
        conditionVariable_host.notify_one();
    }
    
    std::cout << "Streaming Done!" << std::endl;
}

void Client::analyze_raw_data(size_t buffer_sz)
{
}

void Client::process_raw_data(std::vector<std::complex<float>> data_buffer)
{
}


void Client::stop_streaming()
{
    std::cout << "Stopping streaming...showing graphs" << std::endl;
    // stop streaming here
    is_streaming_ = false;
    std::string filename = "usrp_pps_data.dat";
    //Gnuplot gp;

    // open file for reading
    std::ifstream infile(filename, std::ios::binary);

    // loop for reading samples from file
    while (!infile.eof()) 
    {
        // read metadata and samples from file
        md_time_data md;
        infile.read((char*)&md, sizeof(md));
        std::vector<std::complex<short>> samples(10000);
        infile.read((char*)&samples[0], samples.size() * sizeof(std::complex<short>));

        // print out samples with timestamp
        std::cout << "Read " << samples.size() << " samples with timestamp Sec: " << md.full_secs << "frac sec" <<  md.full_secs <<std::endl;
        // TODO: process received samples
    }
    infile.close();
    std::cout << "Streaming stopped." << std::endl;
}

void Client::send_sample(const Sample& sample)
{
    std::ostringstream sample_stream;
    sample_stream << sample.client_id << " " << sample.timestamp << " " << sample.peak << "\n";
    std::string sample_string = sample_stream.str();
    boost::asio::write(socket_, boost::asio::buffer(sample_string));
}

void Client::control_command_handler()
{
    while (true) 
    {
        boost::asio::streambuf command_buffer;
        boost::asio::read_until(socket_, command_buffer, '\n');
        std::istream command_stream(&command_buffer);
        std::string command;
        command_stream >> command;
        if (command == "config") 
        {
            configure_usrp();
        } 
        else if (command == "sync") 
        {
            synchronize_pps();
        } 
        else if (command == "stream") 
        {
            start_streaming();
        } 
        else if (command == "stop") 
        {
            stop_streaming();
        } 
        else 
        {
            std::cout << "Invalid command received from server." << std::endl;
        }
    }
}

void Client::dsp_handler()
{   
    std::unique_lock<std::mutex> lock(mutex_host);
    while (true) 
    {
        conditionVariable_host.wait(lock,[this] {return !is_streaming_;}); //return if is_streaming is still true
        ++stream_pkt_id;

        //create packet
        send_dsp_data();
        //call async send
       
        // sleep for some time before performing next DSP
        std::this_thread::sleep_for(std::chrono::milliseconds(10));//just to give breatihing space.
        is_streaming_ = true; //reset this case as non streaming case has been handled
    }
}

void handleSend(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        // Send operation completed successfully
        // You can perform any necessary actions here
        std::cout << "Send operation completed successfully" << std::endl;
    }
    else
    {
        // Handle send error
        // You can implement appropriate error handling mechanisms here
        std::cerr << "Send error: " << error.message() << std::endl;
    }
}

void Client::send_dsp_data()
{
        std::cout << "Creating header packet" << std::endl;
        //create header
        HeaderPacket header;
        header.packet_id = stream_pkt_id;
        header.pkt_ts = 0;
        header.packet_type = PACKET_TYPE_DATA;

        std::cout << "Creating data packet" << std::endl;
        DataPacket data;
        std::cout << "\tAdding packet ID" << std::endl;
        data.rx_id = 0; //server needs to send the unique id
        std::cout << "\tAdding packet timestamps" << std::endl;
        data.peak_timestamps = &this->peak_timestamp_;
        std::cout << "\tAdding packet timestamps size" << std::endl;
        data.numTimeSamples = data.peak_timestamps->size();

        std::cout << "\tAdding lat" << std::endl;
        data.latitude  = -10.2;//constants
        std::cout << "\tAdding long" << std::endl;
        data.longitude = -11.2;
        std::cout << "\tAdding alt" << std::endl;
        data.altitude  = 10;
        std::cout << "\tAdding data samples" << std::endl;
        data.waveformSamples = &this->waveform_samples_; //not sending the waveform.
        std::cout << "\tPacket created" << std::endl;
        header.packet_length = PacketUtils::DATA_PACKET_FIXED_SIZE+data.peak_timestamps->size()+data.waveformSamples->size();

         
        size_t header_size = PacketUtils::HEADER_PACKET_SIZE;
        std::cout << "Creating packet buffer" << std::endl;
        // Create a packet buffer to store the header packet
        std::vector<char> headerPacketBuffer(header_size);
        PacketUtils::createHeaderPacket(header, headerPacketBuffer);
        
         // Create a packet buffer to store the data packet
        std::size_t packetSize = PacketUtils::DATA_PACKET_FIXED_SIZE 
        + data.peak_timestamps->size() * sizeof(double) 
        + data.waveformSamples->size() * sizeof(std::complex<short>);
        std::vector<char> dataPacketBuffer(packetSize);
        PacketUtils::createDataPacket(data, dataPacketBuffer);

        // Asynchronously send the header
        std::cout << "Sending the packet" << std::endl;
        boost::asio::async_write(socket_, boost::asio::buffer(headerPacketBuffer),
        [&](const boost::system::error_code& error, std::size_t bytes_transferred)
        {
            if (!error)
            {
                // Header sent successfully, now send the data
                    boost::asio::async_write(socket_, boost::asio::buffer(dataPacketBuffer),
                    [&](const boost::system::error_code& error, std::size_t bytes_transferred)
                    {
                        handleSend(error, bytes_transferred);
                    });
            }
            else
            {
                handleSend(error, bytes_transferred);
            }
        });


}


void Client::recv_to_file(void)
{
    uint64_t sim_timestamp_count = 0;
    std::string type                = "short";
    std::string wire_format         = "sc16"; 
    std::string cpu_format          = "sc16";
    size_t channel                  = 0;
    std::string file                = "usrp_pps_data.dat";
    size_t samps_per_buff           = 10000;//samples per buffer;
    size_t num_requested_samples    = 0; //total samples to receive;
    double time_requested           = 5; //total seconds to receive;
    bool bw_summary                 = false;
    bool continue_on_bad_packet     = false;
    bool null                       = false;
    bool enable_size_map            = false;
    bool stats                      = false;
    md_time_data md_time_info       = {};                   

    unsigned long long num_total_samps      = 0;
    static bool stop_signal_called          = false;
    // create a receive streamer
    uhd::stream_args_t stream_args(cpu_format, wire_format);
    std::vector<size_t> channel_nums;
    channel_nums.push_back(channel);
    stream_args.channels                    = channel_nums;
    uhd::rx_streamer::sptr rx_stream        = usrp_handler->get_usrp()->get_rx_stream(stream_args);

    uhd::rx_metadata_t md;
    std::vector<std::complex<short>> buff(samps_per_buff);
    std::ofstream outfile;
    if (not null)
        outfile.open(file.c_str(), std::ofstream::binary);
    bool overflow_message = true;

    // setup streaming
    uhd::stream_cmd_t stream_cmd((num_requested_samples == 0)
                                     ? uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS
                                     : uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps  = size_t(num_requested_samples);
    stream_cmd.stream_now = true;
    stream_cmd.time_spec  = uhd::time_spec_t();
    rx_stream->issue_stream_cmd(stream_cmd);

    typedef std::map<size_t, size_t> SizeMap;
    SizeMap mapSizes;
    const auto start_time = std::chrono::steady_clock::now();
    const auto stop_time =
        start_time + std::chrono::milliseconds(int64_t(1000 * time_requested));
    
    // Track time and samps between updating the BW summary
    auto last_update                     = start_time;
    unsigned long long last_update_samps = 0;

    // Run this loop until either time expired (if a duration was given), until
    // the requested number of samples were collected (if such a number was
    // given), or until Ctrl-C was pressed.
    while (not stop_signal_called
           and (num_requested_samples != num_total_samps or num_requested_samples == 0)
           and (time_requested == 0.0 or std::chrono::steady_clock::now() <= stop_time)) 
           {
                const auto now = std::chrono::steady_clock::now();

                size_t num_rx_samps =
                rx_stream->recv(&buff.front(), buff.size(), md, 3.0, enable_size_map);

                if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) 
                {
                    std::cout << boost::format("Timeout while streaming") << std::endl;
                    break;
                }
                if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW) 
                {
                    if (overflow_message) 
                    {
                        overflow_message = false;
                        std::cerr
                        << boost::format(
                           "Got an overflow indication. Please consider the following:\n"
                           "  Your write medium must sustain a rate of %fMB/s.\n"
                           "  Dropped samples will not be written to the file.\n"
                           "  Please modify this example for your purposes.\n"
                           "  This message will not appear again.\n")
                           % (usrp_handler->get_usrp()->get_rx_rate(channel) * sizeof(std::complex<short>) / 1e6);
                    }
                    continue;
                }
                
                if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) 
                {
                    std::string error = str(boost::format("Receiver error: %s") % md.strerror());
                    if (continue_on_bad_packet) 
                    {
                        std::cerr << error << std::endl;
                        continue;
                    } 
                    else
                    {
                        throw std::runtime_error(error);
                    }
                
                }

                if (enable_size_map) 
                {
                    SizeMap::iterator it = mapSizes.find(num_rx_samps);
                    if (it == mapSizes.end())
                    mapSizes[num_rx_samps] = 0;
                    mapSizes[num_rx_samps] += 1;
                }
                md_time_info.full_secs = md.time_spec.get_full_secs();
                md_time_info.frac_secs = md.time_spec.get_frac_secs();
                stream_seq_id++;
                //Just queue timestamp assuming 
                if(stream_seq_id%333 == 0 && !sim_timestamp_count)
                {
                    peak_timestamp_.push_back((md_time_info.full_secs+md_time_info.frac_secs));
                    if(stream_seq_id == 333)
                    {
                        sim_timestamp_count = 1;
                        std::cout << "Total time sample collected:" << peak_timestamp_.size() <<std::endl;
                    }
                }

#if 0
                std::cout << boost::format(
                             "Received packet: %u samples, %u full secs, %f frac secs")
                             % num_rx_samps % md.time_spec.get_full_secs()
                             % md.time_spec.get_frac_secs()
                      << std::endl;

#endif
                num_total_samps += num_rx_samps;

                if (outfile.is_open()) 
                {
                    outfile.write((char*)&md_time_info, sizeof(md_time_info));
                    outfile.write((const char*)&buff.front(), num_rx_samps * sizeof(std::complex<short>));
                }

            if (bw_summary) 
            {
                last_update_samps += num_rx_samps;
                const auto time_since_last_update = now - last_update;
                if (time_since_last_update > std::chrono::seconds(1)) 
                {
                    const double time_since_last_update_s =
                        std::chrono::duration<double>(time_since_last_update).count();
                    const double rate = double(last_update_samps) / time_since_last_update_s;
                    std::cout << "\t" << (rate / 1e6) << " Msps" << std::endl;
                    last_update_samps = 0;
                    last_update       = now;
                }
            }   
        }
        const auto actual_stop_time = std::chrono::steady_clock::now();

        stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
        rx_stream->issue_stream_cmd(stream_cmd);

        if (outfile.is_open()) 
        {
            outfile.close();
        }

        if (stats)
        {
            std::cout << std::endl;
            const double actual_duration_seconds =
            std::chrono::duration<float>(actual_stop_time - start_time).count();

            std::cout << boost::format("Received %d samples in %f seconds") % num_total_samps
                         % actual_duration_seconds
                  << std::endl;
            const double rate = (double)num_total_samps / actual_duration_seconds;
            std::cout << (rate / 1e6) << " Msps" << std::endl;

        if (enable_size_map) 
        {
            std::cout << std::endl;
            std::cout << "Packet size map (bytes: count)" << std::endl;
            for (SizeMap::iterator it = mapSizes.begin(); it != mapSizes.end(); it++)
                std::cout << it->first << ":\t" << it->second << std::endl;
        }
    }
}
