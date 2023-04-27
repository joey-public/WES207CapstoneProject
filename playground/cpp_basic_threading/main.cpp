include <iostream>
inclide <thread>

void DoWork()
{
}

int main()
{
    std::thread worker(DoWork);

    worker.join();

    std::cin.get();
}
