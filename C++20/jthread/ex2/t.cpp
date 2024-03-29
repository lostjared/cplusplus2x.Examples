/* use GCC 13.2 or greater */
#include <future>
#include <iostream>
#include <stop_token>
#include <thread>

void thread1(std::stop_token token) {
	auto id = std::this_thread::get_id();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << id << " -> finished here..\n";
}

int main() {
	std::jthread t(thread1);
	std::cout << "sleep 3 seconds while thread1: " << t.get_id() << " proc\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
	// destructor will join
	return 0;
}