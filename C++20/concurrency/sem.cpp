// Help for this from: C++20 The Complete Guide
// Chapter 17.2 "Semaphores"

#include<iostream>
#include<queue>
#include<chrono>
#include<thread>
#include<mutex>
#include<semaphore>


int main() {
    std::queue<char> val;
    std::mutex val_mut;
    for(int i = 0; i < 5000; ++i) {
        val.push('a' + (i%('z'-'a')));
    }    
    constexpr int thread_count = 10;
    std::counting_semaphore<thread_count> enabled{0};
    std::vector<std::jthread> pool;
    for(int i = 0; i < thread_count; ++i) {
        pool.push_back(std::jthread{[&enabled, &val, &val_mut, i](std::stop_token st) {

            auto id = std::this_thread::get_id();

            while(!st.stop_requested()) {
                enabled.acquire();
                char v; 
                {
                    std::lock_guard lockg {val_mut};
                    v = val.front();
                    val.pop();
               }
               for(int j = 0; j < 10; ++j)  {
                std::cout << "thread id: " << id << " " << v << "\n";
                auto dur = std::chrono::milliseconds(130) * ((i%3)+1);
                std::this_thread::sleep_for(dur);
               }
               enabled.release();
            }

        }});
    }
    std::cout << "wait 2 seconds " << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "enable 3 parallel threads..\n";
    enabled.release(3);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "enable 2 more\n" << std::flush;
    enabled.release(2);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for(auto &tobj : pool) {
        tobj.request_stop();
    }
    return 0;
}