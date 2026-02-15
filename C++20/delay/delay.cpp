#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<chrono>
#include<thread>

int main(int argc, char **argv) {

	if(argc != 3) {
		std::cerr << "Error incorrect arguments." << std::endl;
		std::cout << "Use: input_tex_file.txt delay" << std::endl;
		return EXIT_FAILURE;
	}

	unsigned int milli = std::stoi(argv[2]);
	if(milli == 0) {
		std::cerr << "Invalid delay\n";
		return EXIT_FAILURE;
	}

	std::fstream file;
	file.open(argv[1], std::ios::in);
	if(!file.is_open()) {
		std::cerr << "Error open file: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	std::string line;
	while(std::getline(file, line)) {
		for(std::string::size_type i = 0; i < line.length(); ++i) {
			std::cout << line[i];
			fflush(stdout);
			std::this_thread::sleep_for(std::chrono::milliseconds(milli));
		}
		std::cout << std::endl;
	}
	file.close();
	return 0;
}
