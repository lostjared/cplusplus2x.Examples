#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>
#include<string_view>

template <typename T>
class Pattern {
    std::vector<T> chain;
    size_t repeat = 0;

  public:
    void push(const T &type) { chain.push_back(type); }
    void echo(std::ostream &out, bool rev) {
        repeat = 1;
        bool c = true;
        auto print_value = [&](const T &item) {
            for (size_t z = 0; z < repeat; ++z) {
                if constexpr (std::is_same_v<T, std::string>) {
                    if (!item.empty()) {
                        auto character = item[0];
                        out << (c ? static_cast<char>(toupper(character))
                                  : static_cast<char>(tolower(character)))
                            << " ";
                    }
                    c = !c;
                } else {
                    out << item << " ";
                }
            }
            ++repeat;
        };

        sort();

        if (!rev) {
            for (const auto &i : chain) {
                print_value(i);
            }
        } else {
            for (const auto &i : std::views::reverse(chain)) {
                print_value(i);
            }
        }
        out << "\n";
    }

    void sort() {
        std::sort(chain.begin(), chain.end(), [](auto x, auto y) { return x < y; });
    }
};

class PatternException {
public:
	PatternException(const std::string_view s) : value{s} {}
	std::string text() { return std::string(value); }
private:
	std::string_view value;	
};


template<typename T>
void read_file(const std::string &filename, Pattern<T> &pattern) {
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	if(!file.is_open()) 
		throw PatternException("Could not read file:\n");

	T type;
	while(file >> type) {
		pattern.push(type);
	}
	file.close();
}

int main(int argc, char **argv) {
    Pattern<std::string> pattern;
    if (argc == 1) {
        std::string line;
        while (std::getline(std::cin, line)) {
            pattern.push(line);
        }
        pattern.sort();
    } else if (argc == 2) {
	    read_file<std::string>(argv[1], pattern);
    }

    std::cout << "Forward:\n";
    pattern.echo(std::cout, false);
    std::cout << "Backward:\n";
    pattern.echo(std::cout, true);
    return EXIT_SUCCESS;
}
