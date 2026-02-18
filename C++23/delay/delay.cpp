#include<iostream>
#include<cstdio>
#include<string>
#include<fstream>
#include<algorithm>
#include<chrono>
#include<thread>
#include<print>

void echo_line_vector(std::vector<std::string> &lines, int milli) {
	for(size_t i = 0; i < lines.size(); ++i) {
		std::string &line = lines[i];
		for(std::string::size_type i = 0; i < line.length(); ++i) {
			std::print("{}", line[i]);
			fflush(stdout);
			std::this_thread::sleep_for(std::chrono::milliseconds(milli));
		}
		std::print("\n");
	}
}

int main(int argc, char **argv) {
	if(argc != 3) {
		std::println(stderr, "Error incorrect arguments.");
		std::println(stderr, "Use: input_tex_file.txt delay");
		return EXIT_FAILURE;
	}
	unsigned int milli = std::stoi(argv[2]);
	if(milli == 0) {
		std::println(stderr, "Invalid delay\n");
		return EXIT_FAILURE;
	}
	std::fstream file;
	file.open(argv[1], std::ios::in);
	if(!file.is_open()) {
		std::println(stderr, "Error open file: {}",argv[1]);
		return EXIT_FAILURE;
	}
	std::vector<std::string> lines;
	std::string line;
	while(std::getline(file, line)) {
		if(file) {
			lines.push_back(line);
		}
	}
	file.close();
	// repeat until
	// break with Ctrl+C
	while(1) {
		echo_line_vector(lines, milli);
	}
	return EXIT_SUCCESS;
}
