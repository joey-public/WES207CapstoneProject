#include <iostream>
#include <thread>
#include <chrono>

// Function to be executed in a separate thread
void threadFunction()
{
    std::cout << "Thread function started" << std::endl;
    // Do some work
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Thread function finished" << std::endl;
}

int main()
{
    // Create a thread object and pass the function as an argument
    std::thread t(threadFunction);

    // Wait for the thread to finish
    t.join();

    std::cout << "Main function finished" << std::endl;
    return 0;
}
