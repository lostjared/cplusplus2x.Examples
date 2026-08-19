#include<iostream>
#include<string>
#include<cstdlib>
#include<vector>
#include<cctype>
#include<type_traits>
#include<fstream>
#include<algorithm>

template<typename T>
class Pattern {
	std::vector<T> chain;
public:
	void push(const T &type) {
		chain.push_back(type);	
	}
	void echo(std::ostream &out) {
		bool c = true;
		size_t repeat = 1;
		for(const auto &i : chain) {
			for(size_t z = 0; z < repeat; ++z) {
				if constexpr(std::is_same_v<T, std::string>) {
			  		if(!i.empty()) {
						char character = i[0];
						out << ((c  == true) ? static_cast<char>(toupper(character)) : static_cast<char>(tolower(character))) << " ";
					}
		        		c = !c;
				} else {
				 	out << i << " ";
				}
			}
			++repeat;
		}
		out << "\n";
	}
};



int main(int argc, char **argv) {
	Pattern<std::string> pattern;
	if(argc == 1) {
		pattern.push("L");
		pattern.push("S");
		pattern.push("D");
	} else if(argc == 2) {
		std::fstream file;
		file.open(argv[1], std::ios::in);
		if(!file.is_open()) {
			std::cerr << "File not found.\n";
			return EXIT_FAILURE;
		}
		std::string line;
		while(file >> line) {
			if(!line.empty())
				pattern.push(line);
		}
		file.close();
	}

	pattern.echo(std::cout);
	return EXIT_SUCCESS;
}

