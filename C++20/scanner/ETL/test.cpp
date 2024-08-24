#include<iostream>
#include<string>
#include"parser.hpp"
#include<fstream>
#include<sstream>
#include"ir.hpp"
#include"codegen.hpp"

void test_parse(const std::string &filename, const std::string &out_file) {
    std::fstream file;
    file.open(filename, std::ios::in);
    if(!file.is_open()) {
        std::cerr << "ETL: Could not open: " << filename << ".\n";
        exit(EXIT_FAILURE);
    }  
    std::ostringstream stream;
    stream << file.rdbuf() << " \n";
    file.close();
    if(stream.str().length()>0) {
        parse::Parser parser(new scan::Scanner(scan::TString(stream.str())));
        if(parser.parse()) {
            auto rootAST = parser.getAST();  
            if (rootAST) {
                parse::IRGenerator irGen;
                auto irCode = irGen.generateIR(rootAST);
                std::cout << "IR code: {\n";
                for (const auto &instr : irCode) {
                    std::cout << "\t" << instr.toString() << "\n";
                }
                std::cout << "}\n";
                codegen::CodeEmitter emiter(irGen.table);
                std::string text = emiter.emit(irCode);
                std::fstream ofile;
                ofile.open(out_file, std::ios::out);
                ofile << text << "\n";
                ofile.close();
                std::cout << "ETL: compiled [" << out_file << "]\n";

            }
        } else {
            std::cerr << "ETL: Parsing failed...\n";
        }
    } else {
        std::cerr << "ETL: Zero bytes to read..\n";
        exit(EXIT_FAILURE);
    }
}