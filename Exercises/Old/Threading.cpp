#include <thread>
#include <Catest.h>
#include <atomic>
#include <iostream>
#include <random>


std::random_device rd;
std::mt19937 gen(rd());

// Define the range for your random numbers (1 to 1500)
std::uniform_int_distribution<int> distribution(30, 50);

void process()
{
    int32_t rand = distribution(gen);
    if (rand == 49)
    {
        rand = 2500;
    }
    std::cout << "Worker doing some work for " << rand << " ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand));

}

int64_t getTimestamp()
{
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

DISABLE_SUITE(threading);
CATEST_F(threading, test)
{
    std::atomic<int64_t> timestamp = getTimestamp();
    std::atomic<bool> threadRunning = true;
    int64_t toleranceMs = 2000;
    std::thread worker([&timestamp, &threadRunning]()
        {
            while (threadRunning)
            {
                process();
                timestamp = getTimestamp();
            }
        });

    bool sendSupervision = true;
    int64_t lastCheckpoint = getTimestamp();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        int64_t now = getTimestamp();
        auto timeSince = now - timestamp;
        std::cout << "\t\t\t\t\t\t\t\tTime since last update: " << timeSince << " ms\n";
        if (timeSince > toleranceMs)
        {
            sendSupervision = false;
        }

        if (sendSupervision)
        {
            std::cout << "\t\t\t\t\t\t\t\tCheckpoint reached!( time since last: "<<now-lastCheckpoint << "ms)" << std::endl;
            lastCheckpoint = getTimestamp();
        }
        if (!sendSupervision)
        {
            std::cout << "\t\t\t\t\t\t\t\tStopping supervision" << std::endl;
            threadRunning = false;
            break;
        }       
    }
    worker.join();
}
