#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

std::string shared_data;
bool data_ready = false;

std::mutex mtx;
std::condition_variable cv;

void receiver() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return data_ready; });
    std::cout << "I RECIEVED THE UNHOLY MESSAGE: " << shared_data << std::endl;
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    std::thread t(receiver);

    {
        std::lock_guard<std::mutex> lock(mtx);
        shared_data = "You are a weak copy of my TRUE POWER!!!";
        data_ready = true;
    }

    cv.notify_one();

    t.join();

    auto end = clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    std::cout << "Thread cummunication time: " << duration.count() << " microseconds\n";

    return 0;
}
