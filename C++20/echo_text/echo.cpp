#include "argz.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

std::string readStringFromFile(const std::string &filename) {
	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file.is_open()) {
		std::cout << "Error could not open file: " << filename << "\n";
		exit(EXIT_FAILURE);
	}
	file.seekg(0, std::ios::end);
	long long int len{file.tellg()};
	file.seekg(0, std::ios::beg);
	std::unique_ptr<char[]> buffer(new char[len + 1]);
	file.read(buffer.get(), len);
	buffer[len] = 0;
	file.close();
	return std::string(buffer.get());
}

int main(int argc, char **argv) {
	Argz<std::string> argz(argc, argv);
	argz.addOptionSingleValue('t', "time delay").addOptionDoubleValue('T', "time", "time delay").addOptionSingleValue('i', "input filename").addOptionDoubleValue('I', "input", "input filename");
	try {
		int value{};
		Argument<std::string> arg;
		std::string delay_time;
		std::string filename;
		while ((value = argz.proc(arg)) != -1) {
			switch (value) {
			case 't':
			case 'T':
				delay_time = arg.arg_value;
				break;
			case 'i':
			case 'I':
				filename = arg.arg_value;
				break;
			}
		}

		if (delay_time.length() == 0) {
			std::cerr << "requires delay..\n";
			argz.help(std::cout);
			return 0;
		}
		if (filename.length() == 0) {
			std::cerr << "requires filename..\n";
			argz.help(std::cout);
			return 0;
		}

		std::string data{readStringFromFile(filename)};
		int delay{atoi(delay_time.c_str())};
		for (const auto &elm : data) {
			std::cout.put(elm).flush();
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}

	} catch (const ArgException<std::string> &e) {
		std::cerr << "Syntax Error: " << e.text() << "\n";
	}

	return 0;
}