#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <syncstream>

std::latch work_start_signal(3);

void worker(int id) {
    std::osyncstream(std::cout) << "[Worker " << id << "] Loading assets...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(id * 100));
    std::osyncstream(std::cout) << "[Worker " << id << "] Ready!\n";
    work_start_signal.count_down(); 
}

int main() {
    std::vector<std::thread> threads;
    for(int i=0; i<3; ++i) threads.emplace_back(worker, i+1);
    std::cout << "[Main] Waiting for workers to be ready...\n";
    work_start_signal.wait();
    std::cout << "[Main] All workers ready! Starting game loop.\n";
    for(auto& t : threads) t.join();
    return 0;
}
