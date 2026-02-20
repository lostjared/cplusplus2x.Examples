#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<cstdlib>
#include<random>
#include<ctime>
#include<print>
#include<algorithm>
#include<vector>
#include<ranges>
#include<set>
#include<iterator>
#include<cctype>
#include"argz.hpp"

std::string reverse_string(const std::string &);
std::string shuffle_string(const std::string &);

template<typename T>
void echo_words(const T &m) {
    for(auto &&value : m) {
        if(!value.empty())
            std::print("{} ", value);
    }
    std::print("\n");
}

template<typename T, typename F>
void echo_words(const T &m, F func, const bool sorted = false) {
    std::vector<std::string> cap;
    cap.resize(m.size());
    for (auto &&value : m | std::views::transform(func)) {
        cap.push_back(value);
    }
    if(sorted) {
        std::sort(cap.begin(), cap.end());
    }
    echo_words(cap);
}

std::string reverse_string(const std::string &text) {
	std::string data(text.rbegin(), text.rend());
	return data;
}

std::string shuffle_string(const std::string &text) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if (text.length() < 4) {
        return text;
    }
    char first = text.front();
    char last = text.back();
    std::string middle = text.substr(1, text.length() - 2);
    std::string original_middle = middle;
    if (std::ranges::all_of(middle, [&](char c) { return c == middle[0]; })) {
        return text;
    }
    std::string reversed_text(text.rbegin(), text.rend());
    bool is_palindrome = (text == reversed_text);
    int attempts = 0;
    do {
        std::shuffle(middle.begin(), middle.end(), gen);
        ++attempts;
        if (attempts >= 1000) {
            middle = original_middle;
            break;
        }
    } while (middle == original_middle);
    std::string shuffled = first + middle + last;
    if (!is_palindrome && shuffled == text) {
        return text;
    }
    return shuffled;
}

template<typename T>
void parse_words(const std::string &s, T out, const int case_mode = 0) {
    size_t i = 0;
    std::string word;
    size_t index = 0;
    while (i < s.length()) {
        char c = s[i++];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            if(case_mode == 0)
                word += c;
            else if(case_mode == 1)
                word += toupper(c);
            else if(case_mode == 2)
                word += tolower(c);
        } else {
            if(!word.empty()) {
                *out++ = word;
                word.clear();
            }
        }
    }
    if(!word.empty())
        *out++ = word;
}

struct Args {
    std::string source_file;
    int mode = 0;
    bool uniq = false;
    bool static_order = false;
    bool sorted_ = false;
    int value_case = 0;
};

int main(int argc, char **argv) {
    Argz<std::string> parser(argc, argv);
    parser.addOptionSingle('h', "Display help message")
        .addOptionSingle('s', "shuffle")
        .addOptionSingle('r', "reverse")
        .addOptionSingle('c', "static order (no shuffle)")
        .addOptionSingle('u', "unique words only")
        .addOptionSingle('o', "sorted")
        .addOptionSingle('n', "no operation keep the same")
        .addOptionSingle('U', "upper case")
        .addOptionSingle('L', "lower case")
        .addOptionSingleValue('i', "input file")
        ;
    Args args;
    Argument<std::string> arg;
	int value = 0;
    try {
        while((value = parser.proc(arg)) != -1) {
            switch(value) {
                case 'h':
                    parser.help(std::cout);
                    break;
                case 'u':
                    args.uniq = true;
                    break;
                case 's':
                    args.mode = 1;
                    break;
                case 'r':
                    args.mode = 2;
                    break;
                case 'n':
                    args.mode = 3;
                    break;
                case 'i':
                    args.source_file = arg.arg_value;
                    break;
                case 'c':
                    args.static_order = true;
                    break;
                case 'o':
                    args.sorted_ = true;
                    break;
                case 'U':
                    args.value_case = 1;
                    break;
                case 'L':
                    args.value_case = 2;
                    break;
            }
        }
    } catch(const ArgException<std::string> &e) {
        std::println(stderr, "Exception: {}", e.text());
        return EXIT_FAILURE;
    } catch(const std::exception &e) {
        std::println(stderr, "std::exception: {}", e.what());
        return EXIT_FAILURE;
    }
    if(args.mode == 0) {
        std::println(stderr, "You must provide an operation option");
        parser.help(std::cout);
        return EXIT_FAILURE;
    }
    if(args.value_case < 0 || args.value_case > 2) {
        std::println(stderr, "Value case must be 0-2");
        return EXIT_FAILURE;
    }
    if(args.source_file.empty()) {
        std::println(stderr, "You must provide a filename");
        parser.help(std::cout);
        return EXIT_FAILURE;
    }
    std::fstream file;
    file.open(args.source_file, std::ios::in);
    if(!file.is_open()) {
        std::println(stderr, "Error: file not found/could not open: {}", argv[1]);
        return EXIT_FAILURE;
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    if(args.uniq == false) {
        std::vector<std::string> words;
        parse_words(stream.str(), std::back_inserter(words), args.value_case);    
        static std::random_device rd;
        static std::mt19937 gen(rd());
        if(!args.static_order)
            std::shuffle(words.begin(), words.end(), gen);
        if(args.mode == 3)
            echo_words(words);
        else
            echo_words(words, (args.mode == 2) ? reverse_string : shuffle_string, args.sorted_);
    } else {
        std::set<std::string> words;
        parse_words(stream.str(), std::inserter(words, words.end()), args.value_case);
        if(args.mode == 3) 
            echo_words(words);
        else
            echo_words(words, (args.mode == 2) ? reverse_string : shuffle_string, args.sorted_);
    }
    return EXIT_SUCCESS;
}
