#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include"argz.hpp"
#include<cstdlib>
#include<sstream>
#include<opencv2/opencv.hpp>

bool convertFile(std::string_view input, std::string_view output, int width, int height);

int main(int argc, char **argv) {
    Argz<std::string> argz;
    argz.initArgs(argc, argv)
    .addOptionSingleValue('i', "input file")
    .addOptionSingleValue('o', "output file format")
    .addOptionSingleValue('s', "scale resolution formst: WithxHeight");

    if(argc == 1) {
        argz.help(std::cout);
        return 0;
    }

    std::string input_file, output_format, image_size;

    try {
        int value {};
        Argument<std::string> arg;
        while ((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'i':
                input_file = arg.arg_value;
                break;
                case 'o':
                output_format = arg.arg_value;
                break;
                case 's':
                image_size = arg.arg_value;
                break;
            }
        }
        if(input_file.length() == 0 || output_format.length() == 0 || image_size.length() == 0) {
            std::cerr << "Error missing arguments...\n";
            argz.help(std::cout);
            return 0;
        }
        const auto pos {image_size.find("x")};
        if(pos == std::string::npos) {
            std::cerr << "Error use format WidthxHeight ex: 640x360\n";
            return 0;
        }
        const std::string left {image_size.substr(0, pos)};
        const std::string right {image_size.substr(pos+1, image_size.length()-pos)};
        if(convertFile(input_file, output_format, atoi(left.c_str()), atoi(right.c_str()))) {
            std::cout << "image_convert: success.\n";
        } else {
            std::cout << "image_convert: failed.\n";
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
    }
    return  0;
}

bool convertFile(std::string_view input, std::string_view output, int width, int height) {
    if(width == 0 || height == 0) {
        std::cerr << "invalid with/height\n";
        return false;
    }
    std::vector<std::string> list_files;
    std::fstream file;
    file.open(input, std::ios::in);
    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
        if(file) {
            list_files.push_back(line);
        }
    }

    int converted {0};

    if(list_files.size() > 0) {
        for(const auto &i : list_files) {
            std::ostringstream stream;
            auto pos {i.rfind(".")};
            std::string fname = i.substr(0, pos);
            if(fname.length() > 0) {
                stream << fname << "." << width << "x" << height << "." << output;
                cv::Mat input {cv::imread(i)};
                if(!input.empty()) {
                    const std::string o_file{stream.str()};
                    std::cout << i << " -> " << o_file << "\n";
                    cv::Mat resized;
                    cv::resize(input, resized, cv::Size(width, height));
                    cv::imwrite(o_file, resized);
                    converted++;
                }
            }
        }
   }
   std::cout << "image(s) converted: " << converted << "\n";
   return true;
}