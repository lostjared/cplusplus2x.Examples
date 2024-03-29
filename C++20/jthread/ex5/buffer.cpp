#include <iostream>
#include <ranges>
#include <stop_token>
#include <string>
#include <thread>
#include <vector>

template <typename F>
void useThreads(char *buffer, int size, int thread_number, F f) {
	std::vector<std::jthread> threads;
	for(int i = 0; i < thread_number; ++i) {
		threads.push_back(std::jthread(f, buffer, i * (size / thread_number), size / thread_number));
	}
	for(int i = 0; i < thread_number; ++i)
		threads[i].join();
}

int main() {
	char buffer[2048];
	useThreads(buffer, 2048, 4, [](std::stop_token token, char *buffer, int start, int size) {
		for(int i = start; i < start + size && !token.stop_requested(); ++i) {
			buffer[i] = rand() % 255;
		}
	});
	int index = 0;
	for(int i = 0; i < 2048; ++i) {
		std::cout << index++ << ": " << static_cast<int>(buffer[i]) << "\n";
	}
	return 0;
}