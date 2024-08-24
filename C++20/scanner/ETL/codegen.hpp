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
            collectStringLiterals(code);
            int stackSize = calculateRequiredStackSize(code);
            emitDataSection(output);
            emitPreamble(output, stackSize);
            emitCode(code, output);
            emitPostamble(output);
            return output.str();
        }

    private:
        symbol::SymbolTable &table;
        int currentStackOffset;
        std::unordered_map<std::string, int> variableOffsets;
        bool init = false;
        std::unordered_map<std::string, std::string> stringLiterals;

        void collectStringLiterals(const ir::IRCode &code) {
            for (const auto &instr : code) {
                if (instr.type == ir::InstructionType::LOAD_CONST && instr.op1[0] == '\"') {
                    std::string label = "str" + std::to_string(stringLiterals.size());
                    stringLiterals[instr.op1] = label;
                }
            }
        }

        int calculateRequiredStackSize(const ir::IRCode &code) {
            int stackSlots = 0;
            for (const auto &instr : code) {
                if (variableOffsets.find(instr.dest) == variableOffsets.end() && !instr.dest.empty()) {
                    stackSlots++;
                    variableOffsets[instr.dest] = stackSlots * -8;
                }
            }
            return stackSlots * 8;
        }

        void emitDataSection(std::ostringstream &output) {
            output << ".section .data\n";
            for (const auto &entry : stringLiterals) {
                output << entry.second << ": .asciz " << ir::escapeString(entry.first) << "\n";
            }
        }

        void emitPreamble(std::ostringstream &output, int stackSize) {
            output << ".section .text\n";
            output << ".globl main\n";
            output << "main:\n";
            output << "    pushq %rbp\n";
            output << "    movq %rsp, %rbp\n";
            if (stackSize > 0) {
                output << "    subq $" << stackSize << ", %rsp\n";
            }
        }

        void emitPostamble(std::ostringstream &output) {
            if (!init) {
                output << "    movq $0, %rax\n";
                output << "    leave\n";
                output << "    ret\n";
            }
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
                    case ir::InstructionType::RETURN:
                        emitReturn(output, instr);
                        break;
                    default:
                        std::cerr << "Unsupported IR Instruction: " << instr.toString() << std::endl;
                        break;
                }
            }
        }

        void emitLoadConst(std::ostringstream &output, const ir::IRInstruction &instr) {
            allocateStackSpace(instr.dest);
            if (instr.op1[0] == '\"') {
                std::string label = stringLiterals[instr.op1];
                output << "    leaq " << label << "(%rip), %rax\n";
                output << "    movq %rax, " << getOperand(instr.dest) << "\n";
            } else {
                output << "    movq $" << instr.op1 << ", %rax\n";
                output << "    movq %rax, " << getOperand(instr.dest) << "\n";
            }
        }

        void emitBinaryOp(std::ostringstream &output, const ir::IRInstruction &instr, const std::string &op) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    " << op << " " << getOperand(instr.op2) << ", %rax\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitDiv(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    cqto\n";
            output << "    idivq " << getOperand(instr.op2) << "\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitAssign(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitLoadVar(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitNeg(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << "    movq " << getOperand(instr.op1) << ", %rax\n";
            output << "    negq %rax\n";
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitCall(std::ostringstream &output, const ir::IRInstruction &instr) {
            static const std::vector<std::string> argumentRegisters = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
            size_t numArgs = instr.args.size();
            for (size_t i = 0; i < numArgs && i < argumentRegisters.size(); ++i) {
                output << "    movq " << getOperand(instr.args[i]) << ", " << argumentRegisters[i] << "\n";
            }
            for (size_t i = numArgs; i > argumentRegisters.size(); --i) {
                output << "    movq " << getOperand(instr.args[i - 1]) << ", %rax\n";
                output << "    pushq %rax\n";
            }
            output << "    call " << instr.functionName << "\n";
            if (numArgs > argumentRegisters.size()) {
                size_t extraArgs = numArgs - argumentRegisters.size();
                output << "    addq $" << (extraArgs * 8) << ", %rsp\n";
            }
            allocateStackSpace(instr.dest);
            output << "    movq %rax, " << getOperand(instr.dest) << "\n";
        }

        void emitLabel(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (instr.dest == "init") {
                init = true;
            }
            output << instr.dest << ":\n";
        }

        void emitReturn(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (!instr.dest.empty()) {
                output << "    movq " << getOperand(instr.dest) << ", %rax\n";
            }
            output << "    leave\n";
            output << "    ret\n";
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