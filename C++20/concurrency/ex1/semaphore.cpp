// Help from C++20: The Complete Guide
// Chapter 17.2
// Uses binary semaphores

#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>

int main() {
	int shared_info{0};
	std::binary_semaphore d_ready{0};
	std::binary_semaphore d_done{0};
	std::jthread process{[&](std::stop_token st) {
		while(!st.stop_requested()) {
			if(d_ready.try_acquire_for(std::chrono::seconds(1))) {
				int data = shared_info;
				std::cout << "process: read " << data << "\n";
				std::this_thread::sleep_for(data * std::chrono::milliseconds(500));
				std::cout << "process: done\n";
				d_done.release();
			} else {
				std::cout << "process timeout..\n";
			}
		}
	}};
	for(int i = 0; i < 10; ++i) {
		std::cout << "main data: " << i << "\n";
		shared_info = i;
		d_ready.release();
		d_done.acquire();
		std::cout << "main data [done]\n";
	}
	return 0;
}