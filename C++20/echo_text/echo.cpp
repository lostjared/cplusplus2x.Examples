#include "argz.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

std::string readStringFromFile(const std::string &name) {
    std::ifstream file(name, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + name);
    }
    auto len{file.tellg()};
    if (len == -1) {
        throw std::runtime_error("Could not get length of the file: " + name);
    }
    std::string temp;
    temp.resize(len);
    file.seekg(0, std::ios::beg);
    file.read(&temp[0], len);
    if (!file) {
        throw std::runtime_error("Error reading file: " + name);
    }
    return temp;
}

int main(int argc, char **argv) {
	try {
		Argz<std::string> argz(argc, argv);
		argz.addOptionSingleValue('t', "time delay").addOptionDoubleValue('T', "time", "time delay").addOptionSingleValue('i', "input filename").addOptionDoubleValue('I', "input", "input filename");
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
	} catch (std::runtime_error &e) {
		std::cerr << "Runtime Error: " << e.what() << "\n";
	}

	return 0;
}