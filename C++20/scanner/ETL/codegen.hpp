#ifndef _CODEGEN_H_
#define _CODEGEN_H_
#include <unordered_map>
#include <stack>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "ir.hpp"  

namespace codegen {

    class CodeEmitter {
    public:
        CodeEmitter(symbol::SymbolTable &symbolTable) : table(symbolTable), currentStackOffset(0) {}

        std::string emit(const ir::IRCode &code) {
            std::ostringstream output;

            emitPreamble(output);
            emitCode(code, output);
            emitPostamble(output);

            return output.str();
        }

    private:
        symbol::SymbolTable &table;
        int currentStackOffset;  
        std::unordered_map<std::string, int> variableOffsets;  

        void emitPreamble(std::ostringstream &output) {
            output << ".section .text\n";
            output << ".globl main\n";
            output << "main:\n";
            output << "    pushq %rbp\n";   
            output << "    movq %rsp, %rbp\n";  
        }

        void emitPostamble(std::ostringstream &output) {
            output << "    movq $0, %rax\n";  
            output << "    leave\n";  
            output << "    ret\n";  
        }

        void emitCode(const ir::IRCode &code, std::ostringstream &output) {
            for (const auto &instr : code) {
                switch (instr.type) {
                    case ir::InstructionType::ADD:
                        emitBinaryOp(output, instr, "addq");
                        break;
                    case ir::InstructionType::SUB:
                        emitBinaryOp(output, instr, "subq");
                        break;
                    case ir::InstructionType::MUL:
                        emitBinaryOp(output, instr, "imulq");
                        break;
                    case ir::InstructionType::DIV:
                        emitDiv(output, instr);
                        break;
                    case ir::InstructionType::ASSIGN:
                        emitAssign(output, instr);
                        break;
                    case ir::InstructionType::LOAD_CONST:
                        emitLoadConst(output, instr);
                        break;
                    case ir::InstructionType::LOAD_VAR:
                        emitLoadVar(output, instr);
                        break;
                    case ir::InstructionType::NEG:
                        emitNeg(output, instr);
                        break;
                    case ir::InstructionType::CALL:
                        emitCall(output, instr);
                        break;
                    case ir::InstructionType::LABEL:
                        emitLabel(output, instr);
                        break;
                    default:
                        std::cerr << "Unsupported IR Instruction: " << instr.toString() << std::endl;
                        break;
                }
            }
        }

        void emitBinaryOp(std::ostringstream &output, const ir::IRInstruction &instr, const std::string &op) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    " << op << " " << getOperand(instr.op2) << ", %rax\n";
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitDiv(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    cqto\n";  // Sign extend RAX into RDX:RAX
            output << "    idivq " << getOperand(instr.op2) << "\n";
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitAssign(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitLoadConst(std::ostringstream &output, const ir::IRInstruction &instr) {
            allocateStackSpace(instr.dest);
            output << "    movq $" << instr.op1 << ", " << getOperand(instr.dest) << "\n";
        }

        void emitLoadVar(std::ostringstream &output, const ir::IRInstruction &instr) {
            allocateStackSpace(instr.dest);
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitNeg(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    negq %rax\n";
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitCall(std::ostringstream &output, const ir::IRInstruction &instr) {
            for (const auto &arg : instr.args) {
                output << "    movq " << getOperand(arg) << ", %rax\n";
                output << "    pushq %rax\n";
            }
            output << "    call " << instr.functionName << "\n";
            output << "    addq $" << instr.args.size() * 8 << ", %rsp\n";  
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitLabel(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << instr.dest << ":\n";
        }

        void allocateStackSpace(const std::string &var) {
            if (variableOffsets.find(var) == variableOffsets.end()) {
                currentStackOffset -= 8;  
                variableOffsets[var] = currentStackOffset;
            }
        }

        std::string getOperand(const std::string &operand) {
            if (variableOffsets.find(operand) != variableOffsets.end()) {
                int offset = variableOffsets[operand];
                return std::to_string(offset) + "(%rbp)";
            }

            std::cerr << "Error: Undefined variable " << operand << std::endl;
            exit(EXIT_FAILURE);
        }
    };
}

#endif
