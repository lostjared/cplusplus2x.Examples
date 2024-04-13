/*
	Crunch Code GPLv3
*/
#include <algorithm>
#include <iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <vector>

void crunch(std::istream &in, std::ostream &out);
std::string remove_ml_comment(const std::string &s1);
void crunch_line(const std::string &s, std::ostream &out);
bool testchr(const std::string, std::size_t &i);

int main(int argc, char **argv) {
	if(argc == 1) {
		crunch(std::cin, std::cout);
	} else {
		for(int i = 1; i < argc; ++i) {
			std::fstream file;
			file.open(argv[i], std::ios::in);
			if(!file.is_open()) {
				std::cerr << "Error could not open: " << argv[i] << "\n";
				continue;
			} else {
				crunch(file, std::cout);
			}
			file.close();
		}
	}
	return 0;
}

void crunch(std::istream &in, std::ostream &out) {
	std::ostringstream stream;
	stream << in.rdbuf();
	std::string data{stream.str()};
	data = remove_ml_comment(data);
	std::istringstream is{data};
	std::vector<std::string> lines;
	while(!is.eof()) {
		std::string line;
		std::getline(is, line);
		if(is && line.length() > 0)
			lines.push_back(line);
	}
	for(std::size_t i = 0; i < lines.size(); ++i) {
		crunch_line(lines[i], out);
		out << " ";
	}
	out << "\n";
}

void crunch_line(const std::string &s, std::ostream &out) {
	bool grab_string{false};
	for(size_t i = 0; i < s.length(); ++i) {
		if(grab_string == false) {
			if(!testchr(s, i)) {
				continue;
			}
			if(s[i] == '/' && i + 1 < s.length() && s[i + 1] == '/')
				return;
		}
		if(s[i] == '\\' && i + 1 < s.length() && s[i + 1] == '\"') {
			out << s[i];
			out << s[i + 1];
			i++;
			continue;
		} else if(s[i] == '\"')
			grab_string = !grab_string;

		out << s[i];
	}
}

bool testchr(const std::string s, std::size_t &i) {
	if(s[i] == '\t' || s[i] == '\r' || s[i] == '\n')
		return false;
	if(s[i] == ' ' && i + 1 < s.length() && s[i + 1] == ' ') {
		i++;
		return false;
	}
	if(s[i] == '\\' && i + 1 < s.length() && s[i + 1] != '\\')
		return false;

	return true;
}

bool chkChr(const std::string &text, std::size_t i, char c) {
	if(i < text.length() && text[i] == c)
		return true;
	return false;
}

std::string remove_ml_comment(const std::string &text) {
	std::string temp{};
	for(std::size_t i = 0; i < text.length(); ++i) {
		if(chkChr(text, i, '/') && chkChr(text, i + 1, '*')) {
			i++;
			do {
				i++;
			} while(!(chkChr(text, i, '*') && chkChr(text, i + 1, '/')));
		} else if(text[i] != '/') {
			temp += text[i];
			continue;
		}
	}
	return temp;
}
