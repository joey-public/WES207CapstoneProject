#include "host.h"
#include <boost/asio/ip/tcp.hpp>
#include <fstream>

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
      usrp_ip_(usrp_ip)
{
      
}

void Client::start()
{
    std::cout << "Connecting to server " << server_address_ << ":" << server_port_ << "..." << std::endl;
    std::cout << "USRP IP Address: " << usrp_ip_ << std::endl;
    boost::asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(server_address_, server_port_);
    socket_.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 33334 ));
    std::cout << "Connected to server." << std::endl;

#if 0
    // Get assigned client ID from server
    boost::asio::streambuf response_buffer;
    boost::asio::read_until(socket_, response_buffer, '\n');
    std::istream response_stream(&response_buffer);
    response_stream >> client_id_;
    std::cout << "Assigned client ID: " << client_id_ << std::endl;
#endif
    boost::asio::write(socket_, boost::asio::buffer("This is Client"));
    control_command_thread_ = std::thread(&Client::control_command_handler, this);
    dsp_thread_ = std::thread(&Client::dsp_handler, this);
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
#if 0
    std::cout << "Current USRP Time frac sec: " << time_spec.get_frac_secs() << std::endl;
    std::cout << "Current USRP Time full sec: " << time_spec.get_full_secs() << std::endl;
    std::cout << "Current USRP Time Real sec: " << time_spec.get_real_secs() << std::endl;
    std::cout << "Current USRP Time Tick count: "<< time_spec.get_tick_count(time_spec.get_frac_secs()) << std::endl;
#endif
    is_synchronized_ = true;
    std::cout << "Synchronized to PPS." << std::endl;
}

void Client::start_streaming()
{
    std::cout << "Starting streaming..." << std::endl;
    // start streaming here
    is_streaming_ = true;
    recv_to_file();
    
    std::cout << "Streaming stopped." << std::endl;
}

void Client::stop_streaming()
{
    std::cout << "Stopping streaming..." << std::endl;
    // stop streaming here
    is_streaming_ = false;

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
        if (command == "configure_usrp") 
        {
            configure_usrp();
        } 
        else if (command == "synchronize_pps") 
        {
            synchronize_pps();
        } 
        else if (command == "start_streaming") 
        {
            start_streaming();
        } 
        else if (command == "stop_streaming") 
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
    while (true) 
    {
        if (is_streaming_) 
        {
            // perform DSP and find peak
            Sample sample;
            sample.client_id = client_id_;
            sample.timestamp = 0.0; // replace with actual timestamp
            sample.peak = 0.0; // replace with actual peak value
            send_sample(sample);
        }
        // sleep for some time before performing next DSP
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void Client::recv_to_file(void)
{
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

                std::cout << boost::format(
                             "Received packet: %u samples, %u full secs, %f frac secs")
                             % num_rx_samps % md.time_spec.get_full_secs()
                             % md.time_spec.get_frac_secs()
                      << std::endl;


                num_total_samps += num_rx_samps;

                if (outfile.is_open()) 
                {
                    outfile.write((char*)&md, sizeof(md));
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