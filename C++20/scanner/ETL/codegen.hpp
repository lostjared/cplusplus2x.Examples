#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <unordered_map>
#include <stack>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "ir.hpp"
#include "symbol.hpp"

namespace codegen {

    class CodeEmitter {
    public:
        CodeEmitter(symbol::SymbolTable &symbolTable, std::unordered_map<std::string, int> &functionVarCount)
            : table(symbolTable), functionLocalVarCount(functionVarCount), currentStackOffset(0), maxStackUsage(0) {}

        std::string emit(const ir::IRCode &code) {
            std::ostringstream output;
            collectStringLiterals(code);
            emitDataSection(output);
            emitPreamble(output);
            emitCallInit(output);
            emitCode(code, output);
            return applyPeephole(output);
        }

    private:
        symbol::SymbolTable &table;
        std::unordered_map<std::string, int> &functionLocalVarCount;
        int currentStackOffset;
        int maxStackUsage;
        std::unordered_map<std::string, int> variableOffsets;
        std::unordered_map<std::string, std::string> stringLiterals;

        void collectStringLiterals(const ir::IRCode &code) {
            for (const auto &instr : code) {
                if (instr.type == ir::InstructionType::LOAD_CONST && instr.op1[0] == '\"') {
                    std::string label = "str" + std::to_string(stringLiterals.size());
                    stringLiterals[instr.op1] = label;
                }
            }
        }

        void emitDataSection(std::ostringstream &output) {
            output << ".section .data\n";
            for (const auto &entry : stringLiterals) {
                output << entry.second << ": .asciz " << ir::escapeString(entry.first) << "\n";
            }
        }

        void emitPreamble(std::ostringstream &output) {
            output << ".section .text\n";
            output << ".globl main\n";
            output << "main:\n";
            output << "    pushq %rbp\n";
            output << "    movq %rsp, %rbp\n";
            output << "    subq $16, %rsp\n";
        }

        void emitCallInit(std::ostringstream &output) {
            output << "    call init\n";
            output << "    movq %rax, %rbx\n";
            output << "    leave\n";
            output << "    movq %rbx, %rax\n";
            output << "    ret\n";
        }

        void emitFunctionPrologue(std::ostringstream &output, int localVariableCount) {
            output << "    pushq %rbp\n";
            output << "    movq %rsp, %rbp\n";
            int stackSpace = ((localVariableCount * 8 + 15) / 16) * 16; // Align to 16 bytes
            output << "    subq $" << stackSpace << ", %rsp\n";
        }

        void emitFunctionEpilogue(std::ostringstream &output) {
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
            if (instr.op1[0] == '\"') {
                std::string label = stringLiterals[instr.op1];
                output << "    leaq " << label << "(%rip), %rax\n";
            } else {
                output << "    movq $" << instr.op1 << ", %rax\n";
            }
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitBinaryOp(std::ostringstream &output, const ir::IRInstruction &instr, const std::string &op) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    " << op << " " << getOperand(instr.op2) << ", %rax\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitDiv(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    cqto\n";
            output << "    idivq " << getOperand(instr.op2) << "\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitAssign(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitLoadVar(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitNeg(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    negq %rax\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitCall(std::ostringstream &output, const ir::IRInstruction &instr) {
            static const std::vector<std::string> argumentRegisters = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
            size_t numArgs = instr.args.size();
            for (size_t i = 0; i < numArgs && i < argumentRegisters.size(); ++i) {
                loadToRegister(output, instr.args[i], argumentRegisters[i]);
            }
            output << "    movq $0, %rax\n"; // Clear RAX for variadic function calls
            output << "    call " << instr.functionName << "\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitLabel(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << instr.dest << ":\n";
            if (instr.dest != "main") {
                int localVarCount = functionLocalVarCount[instr.dest];
                emitFunctionPrologue(output, localVarCount);
            }
        }

        void emitReturn(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (!instr.dest.empty()) {
                output << "    movq " << getOperand(instr.dest) << ", %rax\n";
            } else {
                output << "    movq $0, %rax\n";
            }
            emitFunctionEpilogue(output);
        }

        void loadToRegister(std::ostringstream &output, const std::string &operand, const std::string &reg) {
            if (operand[0] == '$' || operand[0] == '%') {
                output << "    movq " << operand << ", " << reg << "\n";
            } else {
                int offset = getVariableOffset(operand);
                output << "    movq " << offset << "(%rbp), " << reg << "\n";
            }
        }

        void storeToTemp(std::ostringstream &output, const std::string &temp, const std::string &reg) {
            int offset = getVariableOffset(temp);
            output << "    movq " << reg << ", " << offset << "(%rbp)\n";
        }

        int getVariableOffset(const std::string &varName) {
            if (variableOffsets.find(varName) == variableOffsets.end()) {
                currentStackOffset -= 8;
                variableOffsets[varName] = currentStackOffset;
                maxStackUsage = std::min(maxStackUsage, currentStackOffset);
            }
            return variableOffsets[varName];
        }

        std::string getOperand(const std::string &operand) {
            if (operand.empty() || operand[0] == '$' || operand[0] == '%') {
                return operand;
            }
            int offset = getVariableOffset(operand);
            return std::to_string(offset) + "(%rbp)";
        }

        std::string applyPeephole(std::ostringstream &output) {
            std::istringstream stream(output.str());
            std::ostringstream out;
            std::string lastLine;
            std::string line;

            while (std::getline(stream, line)) {
                if (!lastLine.empty() && line == lastLine) {
                    continue; // Skip redundant instruction
                }
                out << line << "\n";
                lastLine = line;
            }

            return out.str();
        }
    };

}

#endif