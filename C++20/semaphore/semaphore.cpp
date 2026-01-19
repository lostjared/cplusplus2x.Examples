#include<iostream>
#include<thread>
#include<semaphore>
#include<string>
#include<syncstream>

std::binary_semaphore waiter(0);
std::string input_data;

void worker_thread() {
   std::osyncstream(std::cout) << "   [Thread] Launched. I am now waiting for the signal...\n";
   waiter.acquire();
   std::osyncstream(std::cout) << "   [Thread] Signal received! I am awake and working.\n";
   std::osyncstream(std::cout) << "   Data entered: " << input_data << "\n";
}

int main() {
    std::thread t(worker_thread);
    std::osyncstream(std::cout) << "[Main] Thread is waiting. Press ENTER to wake it up...\n";
    std::getline(std::cin, input_data);
    std::osyncstream(std::cout) << "[Main] Input received. Releasing semaphore...\n";
    waiter.release();
    t.join();
    std::osyncstream(std::cout) << "[Main] Done.\n";
    return 0;
}
