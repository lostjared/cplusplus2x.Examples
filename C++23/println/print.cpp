#include<thread>
#include<string>
#include<print>
#include<vector>

void testPrint(std::stop_token) {
	std::println("Thread [{}] - Thank you message here", std::this_thread::get_id());
}

int main() {
	std::vector<std::jthread> threads;
	for(int i = 0; i < 10; ++i) {
		threads.push_back(std::jthread(testPrint));
	}
	return 0;
}
