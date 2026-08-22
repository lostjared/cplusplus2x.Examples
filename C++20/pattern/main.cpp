#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include<opencv2/opencv.hpp>



template<typename T>
concept PatternIndex = requires(T type) {
	{ type.size() } -> std::convertible_to<size_t>;
	{ type[0] };
};



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

    size_t size() const { return chain.size(); }
    T &at(size_t index) { return chain[index]; }

    void sort() {
	    std::ranges::sort(chain);
    }

    void shift(size_t pos) {
	    if(chain.empty()) 
		    return;
	    pos %= chain.size();
	    std::ranges::rotate(chain, chain.begin() + pos);
    }

    // use like
    // pattern.mutate([](std::string s){ if(!s.empty()) a[0] ^T= 0x0F; return s; });
    template<typename F>
    void mutate(F &&func) {
	    std::ranges::transform(chain, chain.begin(), std::forward<F>(func));
    }	    
};

class PatternException : public std::runtime_error {
public:
    PatternException(const std::string_view s) : std::runtime_error(std::string(s)) {}
};

template <typename T>
void read_file(const std::string &filename, Pattern<T> &pattern) {
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw PatternException("Could not read file:\n");

    T type;
    while (file >> type) {
        pattern.push(type);
    }
    file.close();
}

// map non random data to cv::Mat as pixels
// was just brainstorming some different ideas
template<PatternIndex T>
void map_pattern(cv::Mat &frame,  Pattern<T> &pattern, cv::Mat &seed) {
	frame = cv::Mat::zeros(720, 1280, CV_8UC3);
	size_t v = 0;
	size_t repeat = 1;
	size_t off = 0;
	for(size_t z = 0; z < frame.rows; ++z) {
		for(size_t i = 0; i < frame.cols; ++i) {
			cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
			cv::Vec3b &pix_seed = seed.at<cv::Vec3b>(z, i);
			T tval = pattern.at(v % pattern.size());
			size_t index = 0;
			for(size_t j = 0; j <= 2 && j < tval.size(); ++j) {
				pixel[index++] = static_cast<unsigned char>((tval[j] * pix_seed[j]) % 256);
			}
			++off;
			if(off > repeat) {
				++v;
				++repeat;
			}
		}
	}
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
	try {
        	read_file<std::string>(argv[1], pattern);
	} catch(const std::runtime_error &e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
    }

    cv::Mat frame;
    cv::Mat img;
    // read exisitng data
    cv::imread("image.png", img);
    //modify with random file junk
    map_pattern<std::string>(frame, pattern, img);
    // write back out to file for next run
    cv::imwrite("image.png", frame);
    /*std::cout << "Forward:\n";
    pattern.echo(std::cout, false);
    std::cout << "Backward:\n";
    pattern.echo(std::cout, true);*/
    return EXIT_SUCCESS;
}
