#include <uhd/usrp/multi_usrp.hpp>
#include <complex>
#include <vector>
#include <thread>
#include <atomic>

int main()
{
    // Set up the USRP and receiver streamer
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(std::string(""));
    uhd::stream_args_t stream_args("fc32");
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    // Set up double buffers
    const size_t packet_size = 1000; // size of each received packet
    std::vector<std::complex<float>> buffer1(packet_size);
    std::vector<std::complex<float>> buffer2(packet_size);
    std::vector<std::complex<float>> *active_buffer = &buffer1;
    std::vector<std::complex<float>> *inactive_buffer = &buffer2;

    // Set up output buffer
    const size_t total_samples = 1000000; // total number of samples to receive
    std::vector<std::complex<float>> output_buffer(total_samples);

    // Start streaming
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);

    // Set up variables for tracking the number of samples received
    std::atomic<size_t> total_samples_received(0);
    std::atomic<bool> stop_requested(false);

    // Create a worker thread to process the inactive buffer
    std::thread worker_thread([&]() {
        while (!stop_requested.load()) {
            // Wait for the active buffer to be filled
            while (total_samples_received.load() + packet_size > active_buffer->size()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // Copy samples from the inactive buffer to the output buffer
            std::copy(inactive_buffer->begin(), inactive_buffer->end(),
                      output_buffer.begin() + total_samples_received.load());

            // Switch buffers
            std::swap(active_buffer, inactive_buffer);
        }
    });

    // Receive samples and store them in the active buffer
    while (total_samples_received.load() < total_samples && !stop_requested.load()) {
        size_t samples_received = rx_stream->recv(active_buffer->data(), active_buffer->size());
        total_samples_received += samples_received;
    }

    // Stop streaming and wait for worker thread to finish
    stop_requested = true;
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);
    worker_thread.join();

    return 0;
}

