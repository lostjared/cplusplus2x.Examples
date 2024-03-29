/* use GCC 13.2 or greater */
#include <future>
#include <iostream>
#include <stop_token>
#include <thread>

void thread1(std::stop_token token) {
	auto id = std::this_thread::get_id();
	while(!token.stop_requested()) {
		std::cout << "thread: " << id << " -> hello world\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << "thread stopping..\n";
}

int main() {
	std::jthread t(thread1);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	t.request_stop();
	t.join();
	return 0;
}