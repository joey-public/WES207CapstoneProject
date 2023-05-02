#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

void streamingThread()
{
    std::cout << "Streaming Thread Launched" << std::endl;
    while(!stop_streaming){}
    std::cout << "Streaming Thread Finished" << std::endl;

}

void processingThread()
{
    std::cout << "Processinging Thread Launched" << std::endl;
    while(!stop_streaming){}
    std::cout << "Processing Thread Finished" << std::endl;
}

int UHD_SAFE_MAIN()
{
    std::cout << "Host Application Launched" << std::endl;
    
    constexpr int ESC_KEY = 27;

    std::atomic<bool> stop_streaming(false);
    
    std::thread s_thread(streamingThread);
    std::thread p_thread(processingThread);
    std::cout << "Press 'ESC' to stop streaming..." << std::endl;

    while (true) {
        if (std::cin.get() == ESC_KEY) { // 27 is the ASCII code for 'ESC'
            stop_streaming = true;
            break;
        }
    }

    s_thread.join();
    p_thread.join();

    std::cout << "Host Application ended" << std::endl;

    return EXIT_SUCESS;
}
