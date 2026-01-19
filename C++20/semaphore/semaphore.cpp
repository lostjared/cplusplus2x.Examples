#include <iostream>
#include <thread>
#include <semaphore>
#include <string>

std::binary_semaphore waiter(0);

void worker_thread() {
    std::cout << "   [Thread] Launched. I am now waiting for the signal...\n";
    waiter.acquire();
    std::cout << "   [Thread] Signal received! I am awake and working.\n";
}

int main() {
    std::thread t(worker_thread);
    std::cout << "[Main] Thread is waiting. Press ENTER to wake it up...\n";
    std::cout << "enter tex: ";
    std::string line;
    std::getline(std::cin, line);
    std::cout << "[Main] Input received. Releasing semaphore...\n";
    waiter.release();
    t.join();
    std::cout << "[Main] Done.\n";
    return 0;
}
