#include<iostream>
#include<string>
#include<format>
#include<vector>
#include<ranges>

void convertStream(std::string_view name, std::istream &in, std::ostream &out);
void stringOutputVector(const std::vector<unsigned char> &v);

int main(int argc, char **argV) {
    if(argc == 1) {
        convertStream("bin_vec", std::cin, std::cout);
    }
    return 0;
}

void convertStream(std::string_view name, std::istream &in, std::ostream &out) {
    out << "inline const std::vector<unsigned char> " << name << " {";
    while(!in.eof()) {
        char c{};
        in.read(&c, sizeof(char));
        std::string hex = std::format("0x{:X}", c);
        if(in)
            out << hex << ",";
        else
            out << hex;
    }
    out << " };\n";
}

void stringOutputVector(const std::vector<unsigned char> &v) {
    for(const auto &e : std::views::all(v)) {
        std::cout << e;
    }
}
