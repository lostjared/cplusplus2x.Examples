#include <iostream>
#include <mutex>
#include <stop_token>
#include <string>
#include <thread>
#include <vector>

int main() {

	std::vector<std::jthread> t_proc;
	std::mutex mut_lock;

	for (int i = 0; i < 5; ++i) {
		t_proc.push_back(std::jthread(
			[i, &mut_lock](std::stop_token token, int id) {
				while (!token.stop_requested()) {
					mut_lock.lock();
					std::cout << "id: " << id << "-> echo..\n";
					mut_lock.unlock();
				}
				std::cout << "thread: " << id << " done!\n";
			},
			i));
	}
	std::this_thread::sleep_for(std::chrono::seconds(3));
	for (auto &t : t_proc) {
		t.request_stop();
	}
	std::cout << "join. on destruct...\n";
}