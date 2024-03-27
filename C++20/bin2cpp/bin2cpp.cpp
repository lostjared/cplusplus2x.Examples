/* Compile with GCC 13.2 or greater */

#include "argz.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

void convertStreamToVector(std::string_view name, std::istream &in, std::ostream &out);
void convertStreamToArray(std::string_view name, std::string_view data, std::ostream &out);
void stringOutputVector(const std::vector<unsigned char> &v);
template <std::size_t N>
void stringOutputArray(std::array<unsigned char, N> &a);

int main(int argc, char **argv) {
	if (argc == 1) {
		convertStreamToVector("bin_vec", std::cin, std::cout);
		return EXIT_SUCCESS;
	}
	try {
		Argz<std::string> argz(argc, argv);
		argz.addOptionSingleValue('i', "input file").addOptionDoubleValue('I', "input", "input file").addOptionSingleValue('o', "output").addOptionDoubleValue('O', "output", "output file").addOptionSingle('h', "help").addOptionDouble('H', "help", "help message").addOptionSingleValue('v', "variable name").addOptionDoubleValue('V', "variable", "variable name");
		Argument<std::string> arg;
		int value{};
		std::string input_file, output_file, variable_name{"bin"};
		;
		while ((value = argz.proc(arg)) != -1) {
			switch (value) {
			case 'i':
			case 'I':
				input_file = arg.arg_value;
				break;
			case 'o':
			case 'O':
				output_file = arg.arg_value;
				break;
			case 'H':
			case 'h':
				argz.help(std::cout);
				return EXIT_SUCCESS;
			case 'v':
			case 'V':
				variable_name = arg.arg_value;
				break;
			}
		}
		if (input_file.length() == 0) {
			std::cerr << "Error input file not specified...\n";
			argz.help(std::cout);
			return EXIT_SUCCESS;
		} else {
			std::size_t len{};
			std::fstream file;
			file.open(input_file, std::ios::in | std::ios::binary);
			if (!file.is_open()) {
				std::cerr << "Could not open file: " << input_file << "\n";
				return EXIT_FAILURE;
			}
			file.seekg(0, std::ios::end);
			len = file.tellg();
			file.seekg(0, std::ios::beg);
			char *buf = new char[len + 1];
			file.read(buf, len);
			std::string data = buf;
			delete[] buf;
			file.close();
			if (output_file.length() == 0)
				convertStreamToArray(variable_name + "_arr", data, std::cout);
			else {
				std::fstream file;
				file.open(output_file, std::ios::out);
				if (!file.is_open()) {
					std::cerr << "Error could not open output file..\n";
					return EXIT_FAILURE;
				}
				file << "#ifndef __ARR_H_HPP_\n";
				file << "#define __ARR_H_HPP_\n";
				convertStreamToArray(variable_name + "_arr", data, file);
				file << "#endif\n";
				file.close();
			}
		}
	} catch (const ArgException<std::string> &e) {
		std::cerr << "Syntax Error: " << e.text() << "\n";
	}

	return 0;
}

void convertStreamToVector(std::string_view name, std::istream &in, std::ostream &out) {
	out << "inline const std::vector<unsigned char> " << name << " {";
	std::size_t index = 0;

	while (!in.eof()) {
		uint8_t c{};
		in.read(reinterpret_cast<char *>(&c), sizeof(uint8_t));
		const std::string hex{std::format("0x{:X}", c)};
		if (in)
			out << hex << ",";
		else
			out << hex;
	}
	out << "};\n";
}

void convertStreamToArray(std::string_view name, std::string_view data, std::ostream &out) {
	out << "inline const std::array<unsigned char, " << data.length() + 1 << "> " << name << " {";
	for (const auto &elm : std::views::all(data)) {
		const std::string hex{std::format("0x{:X}", elm)};
		out << hex << ",";
	}
	const std::string hex{std::format("0x{:X}", 0x0)};
	out << hex << "};\n";
}

void stringOutputVector(const std::vector<unsigned char> &v) {
	for (const auto &e : std::views::all(v)) {
		std::cout << e;
	}
}

template <std::size_t N>
void stringOutputArray(std::array<unsigned char, N> &a) {
	for (const auto &i : std::views::all(a)) {
		std::cout << i;
	}
}