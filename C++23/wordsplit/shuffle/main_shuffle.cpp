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

std::string reverse_string(const std::string &);
std::string shuffle_string(const std::string &);

template<typename T>
void echo_words(T &m) {
    for(auto &i : m) {
        std::print("{} ", shuffle_string(i));
    }
    std::print("\n");
}

std::string reverse_string(const std::string &text) {
	std::string data;
	for(int i = text.length() -1; i >= 0; --i) {
		data += text[i];
	}
	return data;
}

std::string shuffle_string(const std::string &text) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::string data;
    if(text.length() >= 3) {
        char first, last;
        first = text[0];
        last =  text[text.length()-1];
        std::vector<char> ch;
        for(size_t i = 1; i < text.length()-1; ++i) {
            ch.push_back(text[i]);
        }
        std::shuffle(ch.begin(), ch.end(), gen);
        data += first;
        for(size_t i = 0; i < ch.size(); ++i) {
            data += ch[i];
        }
        data += last;
    } else {
        data = text;
    }
    return data;
}

int main(int argc, char **argv) {

    if(argc != 2) {
        std::println("invalid args.");
        return EXIT_FAILURE;
    }

    std::fstream file;
    file.open(argv[1], std::ios::in);
    if(!file.is_open()) {
        std::println("Error: file not found/could not open: {}", argv[1]);
        return EXIT_FAILURE;
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    std::vector<std::string> words;
    size_t i = 0;
    std::string s = stream.str();
    std::string word;
    size_t index = 0;

    while (i < s.length()) {
        if((s[i] >= 'a' && s[i] && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) {
            word += s[i];
            ++i;
            continue;
        } else {
            ++i;
            if(!word.empty())
                words.push_back(word);
            word = "";
            continue;
        }
        ++i;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(words.begin(), words.end(), gen);
    echo_words(words);
    return 0;
}
