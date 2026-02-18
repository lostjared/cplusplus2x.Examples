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

template<typename T, typename F>
void echo_words(T &m, F func) {
    for(auto &i : m) {
        std::print("{} ", func(i));
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

bool compare_vector(const std::vector<char> &v1, const std::vector<char> &v2) {
    if(v1.size() != v2.size()) return false;
    for(size_t i = 0; i < v1.size(); ++i) {
        if(v1[i] != v2[2])
            return false;
    }
    return true;
}

bool is_palindrome(const std::string &text) {
    return text == reverse_string(text);
}

std::string shuffle_string(const std::string &text) {
    static std::random_device rd;
    static std::mt19937 gen(rd());  
    std::string data;
    if(text.length() >= 4) {
        char first, last;
        first = text[0];
        last =  text[text.length()-1];
        std::vector<char> ch;
        std::string sub_p;
        for(size_t i = 1; i < text.length()-1; ++i) {
            sub_p += text[i];
            ch.push_back(text[i]);
        }
        std::string sub_a;
        int attempts = 0;
        bool is_pal = is_palindrome(text);
        do {
            std::shuffle(ch.begin(), ch.end(), gen);
            sub_a = "";
            for(size_t i = 0; i < ch.size(); ++i) {
                sub_a += ch[i];
            }
            ++attempts;
            bool valid = (sub_p != sub_a);
            if(sub_p.length() <= 2 && is_palindrome(sub_p)) {
                 valid = true;
            } else {
                if(valid && !is_pal && ch.size() > 2) {
                    valid = (reverse_string(sub_p) != sub_a);
                }
            }
            if(valid) {
                break;
            }
            if(attempts >= 1000) {
                sub_a = sub_p;  
                break;
            }
        } while(true);
        data += first;
        data += sub_a;
        data += last;
    } else {
        data = text;
    }
    return data;
    
}

int main(int argc, char **argv) {

    if(argc <= 2) {
        std::println("invalid args.");
        return EXIT_FAILURE;
    }

    int mode = 1;

    if(argc == 3) {
        if(std::string(argv[2]) == "r")
            mode = 1;
        else if(std::string(argv[2]) == "s")
            mode = 2;
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
    echo_words(words, (mode == 1) ? reverse_string : shuffle_string);
    return 0;
}
