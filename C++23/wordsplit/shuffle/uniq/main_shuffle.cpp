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
#include<unordered_set>

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
    auto is_palindrome = [](const std::string &text) { return text == reverse_string(text); };
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
        bool valid = false;
        do {
            std::shuffle(ch.begin(), ch.end(), gen);
            sub_a = "";
            for(size_t i = 0; i < ch.size(); ++i) {
                sub_a += ch[i];
            }
            ++attempts;
            valid = (sub_p != sub_a);
            if(sub_p.length() <= 2 && is_palindrome(sub_p)) {
                 valid = true;
            } else {
                if(valid && !is_pal && ch.size() > 2) {
                    valid = (reverse_string(sub_p) != sub_a);
                }
            }
            if(attempts >= 1000) {
                sub_a = sub_p;  
                break;
            }
        } while(!valid);
        data += first;
        data += sub_a;
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
    std::unordered_set<std::string> words;
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
                words.insert(word);
            word = "";
            continue;
        }
        ++i;
    }
    std::vector<std::string> shuffled;
    shuffled.resize(words.size());
    std::copy(words.begin(), words.end(), shuffled.begin());
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), gen);
    echo_words(shuffled);
    return 0;
}
