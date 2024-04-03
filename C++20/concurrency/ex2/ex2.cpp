// Help from C++20: The Complete Guide
// Chapter 17.2
// Expanded Example Uses binary semaphores

#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <deque>

int main() {
	std::deque<int> dq;
	std::binary_semaphore d_ready{0};
	std::binary_semaphore d_done{0};
	std::jthread process{[&](std::stop_token st) {
		while(!st.stop_requested()) {
			if(d_ready.try_acquire_for(std::chrono::seconds(1))) {
                std::deque<int> temp;
                if(!dq.empty()) {// data to read
                    std::copy(dq.begin(), dq.end(), std::front_inserter(temp));
                    dq.erase(dq.begin(), dq.end());
				    std::cout << "process: read ";
                    for(const auto &ix : temp) {
                        std::cout << ix << " ";
                    }
                    std::cout << "\n";
				    std::this_thread::sleep_for(std::chrono::milliseconds(500));
				    std::cout << "process: done\n";
                } else { 
                    std::cout << "No data to read...\n";
                }
				d_done.release();
			} else {
				std::cout << "process timeout..\n";
			}
		}
	}};
	for(int i = 0; i < 25; ++i) {
	   std::cout << "main data: " << i << "\n";
       for(int j = 0; j < rand()%100; ++j) {
            dq.push_front(j);
        }
        if(!dq.empty()) {
            d_ready.release();
		    d_done.acquire();
		    std::cout << "main data [done]\n";
        } else {
            std::cout << "main data [empty]\n";
        }
	}
	return 0;
}