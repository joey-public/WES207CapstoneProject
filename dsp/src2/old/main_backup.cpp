#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/stream.hpp>
#include <boost/format.hpp>
#include <vector>
#include <atomic>
#include <iostream>
#include <thread>

uhd::usrp::multi_usrp::sptr config_usrp()
{
    // Create a USRP object and set the parameters
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make("192.168.11.2");
    usrp->set_rx_subdev_spec("");
    usrp->set_rx_antenna("");
    // Set the clock and time source to external
    usrp->set_clock_source("external");
    usrp->set_time_source("external");
    // Enable automatic gain control
    usrp->set_rx_agc(true);
    // Set the frequency to 900 MHz
    uhd::tune_request_t tune_request(900e6);
    usrp->set_rx_freq(tune_request);
    // Set the bandwidth to 1 MHz
    usrp->set_rx_bandwidth(1e6);
    //set the sampling rate
    usrp->set_rx_rate(1e6);
    // Set the receiver gain range to 0-60 dB
    usrp->set_rx_gain_range(0, 60);
    // Set the streaming parameters
    uhd::stream_args_t stream_args("fc32");
    stream_args.channels = std::vector<size_t>{0}; // stream from channel 0
    stream_args.cpu_format = "fc32"; // 32-bit floating point IQ samples
    usrp->set_stream_args(stream_args);
    // Enable time synchronization with the PPS signal
    usrp->set_time_sync("pps");

    return usrp;
}

void stream_data(uhd::usrp::multi_usrp::sptr usrp, std::vector<float>& data, size_t num_samples, std::atomic<bool>& stop_streaming)
{
    // Create a receive streamer
    uhd::stream_args_t stream_args("fc32");
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    // Start the streamer
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    rx_stream->issue_stream_cmd(stream_cmd);

    // Read samples from the streamer
    std::vector<std::complex<float>> iq_samples(num_samples);
    size_t total_samples = 0;
    while (!stop_streaming && total_samples < num_samples) {
        size_t num_rx_samples = rx_stream->recv(&iq_samples[0], num_samples - total_samples);
        for (size_t i = 0; i < num_rx_samples; i++) {
            data.push_back(std::abs(iq_samples[i]));
        }
        total_samples += num_rx_samples;
    }

    // Stop the streamer
    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);
}

int UHD_SAFE_MAIN(int argc, char *argv[]) 
{
    std::atomic<bool> stop_streaming(false);
    std::vector<float> data;
    size_t num_samples = 100000;

    // Configure the USRP
    uhd::usrp::multi_usrp::sptr usrp = config_usrp();

    // Create a thread to stream data
    std::thread streaming_thread(stream_data, usrp, std::ref(data), num_samples, std::ref(stop_streaming));

    //Create a thread for processing data
    
    //Create a thread for networking

    // Wait for user input to stop streaming
    std::cout << "Press 'ESC' to stop streaming..." << std::endl;
    while (true) {
        if (std::cin.get() == 27) { // 27 is the ASCII code for 'ESC'
            stop_streaming = true;
            break;
        }
    }

    // Join the streaming thread and exit the program
    streaming_thread.join();
    return EXIT_SUCCESS;
}
