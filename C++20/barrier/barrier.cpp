#include<iostream>
#include<thread>
#include<vector>
#include<barrier> 
#include<syncstream>
#include<chrono>

auto on_completion = []() noexcept { 
    static int phase_count = 1;
    std::osyncstream(std::cout) << "\n--- [Barrier] Phase " << phase_count++ << " Completed! Resetting... ---\n\n"; 
};

std::barrier sync_point(3, on_completion);

void worker(int id) {
    std::osyncstream(std::cout) << "[Worker " << id << "] Working on Phase 1...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(id * 200)); // Varied work time
    std::osyncstream(std::cout) << "[Worker " << id << "] Finished Phase 1. Waiting at barrier.\n";
    sync_point.arrive_and_wait();
    std::osyncstream(std::cout) << "[Worker " << id << "] Working on Phase 2...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sync_point.arrive_and_wait();
    std::osyncstream(std::cout) << "[Worker " << id << "] Done with everything.\n";
}

int main() {
    std::cout << "[Main] Spawning 3 workers...\n";
    
    std::vector<std::thread> threads;
    for(int i = 0; i < 3; ++i) {
        threads.emplace_back(worker, i + 1);
    }

    for(auto& t : threads) {
        t.join();
    }
    
    std::cout << "[Main] All threads joined.\n";
    return 0;
}
