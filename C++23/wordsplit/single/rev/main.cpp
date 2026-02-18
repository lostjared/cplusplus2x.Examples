#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<set>
#include<cstdlib>
#include<random>
#include<ctime>
#include<print>

std::string reverse_string(const std::string &text) {
	std::string data;
	for(int i = text.length() -1; i >= 0; --i) {
		data += text[i];
	}
	return data;
}

void echo_words(std::set<std::string> &m) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    while(m.size() > 0) {
        std::uniform_int_distribution<> dis(0, m.size()-1);
        int index = dis(gen);
        auto pos = m.begin();
        std::advance(pos, index);
        if(!pos->empty())
	      std::print("{} ", reverse_string(*pos));

        m.erase(pos);
    }
    std::print("\n");
}

int main(int argc, char **argv) {

    if(argc != 2) {
        std::println("invalid args.");
        return EXIT_FAILURE;
    }
    
    std::fstream file;
    file.open(argv[1], std::ios::in);
    std::ostringstream stream;
    stream << file.rdbuf();
    std::set<std::string> word_map;
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
                word_map.insert(word);
            word = "";
            continue;
        }
        ++i;
    }

    echo_words(word_map);
    return 0;
}
