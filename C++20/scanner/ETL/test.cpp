#include<iostream>
#include<string>
#include"parser.hpp"
#include<fstream>
#include<sstream>
#include"ir.hpp"
#include"ir_opt.hpp"
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
    scan::TString p(stream.str());
    std::ostringstream code;
    bool in_string = false;
    while (1) {
        auto c = p.getch();
        if (c.has_value()) {
            if (*c == '"') {
                in_string = !in_string;
                code << *c;
            } else if (*c == '#' && !in_string) {
                do {
                    c = p.getch();
                } while (c.has_value() && *c != '\n');
            } else {
                code << *c;
            }
        } else break;
    }
    if(stream.str().length()>0) {
        parse::Parser parser(new scan::Scanner(scan::TString(code.str())));
        if(parser.parse()) {
            auto rootAST = parser.getAST();  
            if (rootAST) {
                try {
                        parse::IRGenerator irGen;
                        auto irContext = irGen.generateIR(rootAST);  
                        ir::IROptimizer opt;
                        irContext.instructions = std::move(opt.optimize(irContext.instructions));
                        std::cout << "ETL: IR code: {\n";
                        for (const auto &instr : irContext.instructions) {
                            std::cout << "\t" << instr.toString() << "\n";
                        }
                        codegen::CodeEmitter emitter(irContext.table, irContext.functionLocalVarCount);
                        std::string text = emitter.emit(irContext.instructions);
                        std::fstream ofile;
                        ofile.open(out_file, std::ios::out);
                        ofile << text << "\n";
                        ofile.close();
                        
                        std::cout << "}\n";
                        std::cout << "ETL: compiled [" << out_file << "]\n";
                        exit(EXIT_SUCCESS);
                    } catch(ir::IRException &e) {
                    std::cerr << "ETL: IR Exception: " << e.why() << "\n";
                    exit(EXIT_FAILURE);
                } catch(...) {
                    std::cerr << "ETL: Unknown Exception\n";
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            std::cerr << "ETL: Parsing failed...\n";
        }
    } else {
        std::cerr << "ETL: Zero bytes to read..\n";
        exit(EXIT_FAILURE);
    }
}