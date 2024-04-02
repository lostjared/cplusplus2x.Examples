#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

void crunch(std::istream &in, std::ostream &out);
void crunch_line(const std::string &s, std::ostream &out);

int main() {
    crunch(std::cin, std::cout);
    return 0;
}

void crunch(std::istream &in, std::ostream &out) {
    std::vector<std::string> lines;
    while(!in.eof()) {
        std::string line;
        std::getline(in, line);
        if(in && line.length() > 0) 
            lines.push_back(line);
    }
    for(std::size_t i = 0; i < lines.size(); ++i) {
        crunch_line(lines[i], out);
        out << " ";
    }
    out << "\n";
}

void crunch_line(const std::string &s, std::ostream &out) {
    for(size_t i = 0; i < s.length(); ++i) {
        if(s[i] == '\t' || s[i] == '\r' || s[i] == '\n')
            continue;
        if(s[i] == ' ' && i+1 < s.length() && s[i+1] == ' ') {
            i++;
            continue;
        }
        if(s[i] == '\\' && i+1 < s.length() && s[i+1] != '\\')
            continue;

        out << s[i];
    }
}