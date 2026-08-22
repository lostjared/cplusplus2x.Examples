#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

template <typename T>
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
                        fflush(stdout);
                    }
                    c = !c;
                } else {
                    out << item << " ";
                }
            }
            ++repeat;
        };

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
        if (chain.empty())
            return;
        pos %= chain.size();
        std::ranges::rotate(chain, chain.begin() + pos);
    }

    // use like
    // pattern.mutate([](std::string s){ if(!s.empty()) a[0] ^T= 0x0F; return s; });
    template <typename F>
    void mutate(F &&func) {
        std::ranges::transform(chain, chain.begin(), std::forward<F>(func));
    }
};

class PatternException : public std::runtime_error {
  public:
    PatternException(const std::string_view s) : std::runtime_error(std::string(s)) {}
};

template <PatternIndex T>
void read_stream(std::istream &out, Pattern<T> &pattern) {
    T type;
    while (out >> type) {
        pattern.push(type);
    }
}

template <PatternIndex T>
void read_file(const std::string &filename, Pattern<T> &pattern) {
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw PatternException("Could not read file:\n");
    read_stream(file, pattern);
    file.close();
}

// map non random data to cv::Mat as pixels
// was just brainstorming some different ideas
template <PatternIndex T>
void map_pattern(cv::Mat &frame, Pattern<T> &pattern, cv::Mat &seed) {
    frame = cv::Mat::zeros(720, 1280, CV_8UC3);
    size_t v = 0;
    size_t repeat = 1;
    size_t off = 0;
    for (size_t z = 0; z < frame.rows; ++z) {
        for (size_t i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b &pix_seed = seed.at<cv::Vec3b>(z, i);
            T tval = pattern.at(v % pattern.size());
            size_t index = 0;
            for (size_t j = 0; j <= 2 && j < tval.size(); ++j) {
                pixel[index++] ^= static_cast<unsigned char>((tval[j] ^ pix_seed[j]) % 256);
            }
            ++off;
            if (off > repeat) {
                ++v;
                ++repeat;
            }
        }
    }
}

template <PatternIndex T>
void write_pattern_to_image(std::string_view input_file, std::string_view output_file, Pattern<T> &pattern) {
    cv::Mat frame;
    cv::Mat img;
    // read exisitng data
    cv::imread(std::string(input_file), img);
    if (img.empty())
        throw PatternException("Could not open input image.");
    // modify with random file junk
    map_pattern<std::string>(frame, pattern, img);
    // write back out to file for next run
    cv::imwrite(std::string(output_file), frame);
    std::cout << "Wrote: " << output_file << " \n";
}

int main(int argc, char **argv) {
    Pattern<std::string> pattern;
    if (argc == 4) {
        try {
            read_file<std::string>(argv[1], pattern);
            write_pattern_to_image<std::string>(argv[2], argv[3], pattern);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << "\n";
            return EXIT_FAILURE;
        }
        if (argc == 4) {
        } else {
            pattern.echo(std::cout, false);
        }
    } else if (argc == 3) {

    } else {
        try {
            read_stream<std::string>(std::cin, pattern);
            pattern.echo(std::cout, false);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << "\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
