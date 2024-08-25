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

    struct ValueState {
        bool inRegister;
        bool inMemory;
        std::string registerName;
        int memoryOffset;

        ValueState() : inRegister(false), inMemory(false), registerName(""), memoryOffset(0) {}
    };

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
            return output.str();
            //return applyPeephole(output);
        }

        std::string applyPeephole(std::ostringstream &output) {
            std::istringstream stream(output.str());
            std::ostringstream out;
            int reg = 0;
            std::string r;
            std::string line;

            while (std::getline(stream, line)) {
                auto movqPos = line.find("movq");
                if (movqPos != std::string::npos) {
                    auto rightStart = movqPos + 5;
                    auto commaPos = line.find(",", rightStart);
                    if (commaPos != std::string::npos) {
                        std::string left = line.substr(rightStart, commaPos - rightStart);
                        std::string rightof = line.substr(commaPos + 1);

                        if (!left.empty() && left[0] == '%') {
                            r = left;
                            reg = 1;
                            out << line << "\n";
                        } else if(!left.empty() && left[0] == '$') {
                            r = rightof;
                            reg = 1;
                            out << line << "\n";
                        } 
                        else if (reg == 1 && rightof.find("(") != std::string::npos && rightof.find(left) == std::string::npos) {
                            reg = 0;
                            continue;
                        }
                    }
                } else {
                    reg = 0;
                    out << line << "\n";
                }
            }
            return out.str();
        }
    private:
        symbol::SymbolTable &table;
        std::unordered_map<std::string, int> &functionLocalVarCount;
        int currentStackOffset;
        int maxStackUsage;
        std::unordered_map<std::string, int> variableOffsets;
        std::unordered_map<std::string, ValueState> valueStateMap;
        std::unordered_map<std::string, std::string> stringLiterals;

        std::unordered_map<std::string, std::string> registerValueCache;

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
            output << "    subq $16, %rsp\n";  // Align stack to 16 bytes for safety
        }

        void emitCallInit(std::ostringstream &output) {
            output << "    call init\n";
            output << "    movq %rax, %rbx\n";
            output << "    leave\n";
            output << "    movq %rbx, %rax\n";
            output << "    ret\n";
        }

        void emitFunctionPrologue(std::ostringstream &output, int localVariableCount) {
            int requiredStackSpace = localVariableCount * 8; // Only allocate what's needed
            if (requiredStackSpace > 0) {
                output << "    subq $" << requiredStackSpace << ", %rsp\n";
            }
        }

        void emitFunctionEpilogue(std::ostringstream &output) {
            output << "    leave\n";
            output << "    ret\n";
        }

        void emitCode(const ir::IRCode &code, std::ostringstream &output) {
            currentStackOffset = 0;  // Reset stack offset tracking for each function
            maxStackUsage = 0;       // Reset max stack usage
            variableOffsets.clear(); // Clear variable offsets
            valueStateMap.clear();   // Clear value states

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
                output << "    leaq " << label << "(%rip), %rdi\n";  // Load address of format string into %rdi
            } else {
                output << "    movq $" << instr.op1 << ", %rax\n";
                storeToStack(output, instr.dest, "%rax");  // Store the constant into stack slot
                registerValueCache["%rax"] = instr.op1; // Update cache
            }
        }

        void emitBinaryOp(std::ostringstream &output, const ir::IRInstruction &instr, const std::string &op) {
            loadToRegister(output, instr.op1, "%rax");
            loadToRegister(output, instr.op2, "%rbx");
            output << "    " << op << " %rbx, %rax\n";
            storeToStack(output, instr.dest, "%rax");
            registerValueCache["%rax"] = instr.dest; // Update cache
        }

        void emitDiv(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    cqto\n";
            loadToRegister(output, instr.op2, "%rbx");
            output << "    idivq %rbx\n";
            storeToStack(output, instr.dest, "%rax");
            registerValueCache["%rax"] = instr.dest; // Update cache
        }

        void emitAssign(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (instr.dest == instr.op1) {
                return; // Skip redundant assignment
            }
            loadToRegister(output, instr.op1, "%rax");
            storeToStack(output, instr.dest, "%rax");
            registerValueCache["%rax"] = instr.dest; // Update cache
        }

        void emitLoadVar(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            storeToStack(output, instr.dest, "%rax");
            registerValueCache["%rax"] = instr.dest; // Update cache
        }

        void emitNeg(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    negq %rax\n";
            storeToStack(output, instr.dest, "%rax");
            registerValueCache["%rax"] = instr.dest; // Update cache
        }

        void emitCall(std::ostringstream &output, const ir::IRInstruction &instr) {
            static const std::vector<std::string> argumentRegisters = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
            size_t numArgs = instr.args.size();
            for (size_t i = 0; i < numArgs && i < argumentRegisters.size(); ++i) {
                loadToRegister(output, instr.args[i], argumentRegisters[i]);
            }
            output << "    movq $0, %rax\n"; // Clear %rax for syscall convention (variadic functions)
            output << "    call " << instr.functionName << "\n";
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
                loadToRegister(output, instr.dest, "%rax");
            } else {
                output << "    movq $0, %rax\n";
            }
            emitFunctionEpilogue(output);
        }

        // Load a variable into a register, using value caching to avoid redundant loads
        void loadToRegister(std::ostringstream &output, const std::string &variable, const std::string &reg) {
            auto &state = valueStateMap[variable];
            if (state.inRegister && state.registerName == reg) {
                return;  // Already in the desired register
            }

            // Check cache to avoid redundant loads
            if (registerValueCache[reg] == variable) {
                return;
            }

            if (state.inMemory) {
                output << "    movq " << state.memoryOffset << "(%rbp), " << reg << "\n";
                state.inRegister = true;
                state.registerName = reg;
                registerValueCache[reg] = variable; // Update cache
            } else {
                state.inRegister = true;
                state.registerName = reg;
                registerValueCache[reg] = variable; // Update cache
            }
        }

        // Store a value into the stack, optimizing redundant store cases
        void storeToStack(std::ostringstream &output, const std::string &variable, const std::string &reg) {
            auto &state = valueStateMap[variable];

            // Check if the value is already stored in the memory location
            if (state.inMemory && state.memoryOffset == variableOffsets[variable] && state.registerName == reg) {
                return; // Already stored in the correct location
            }

            if (variableOffsets.find(variable) == variableOffsets.end()) {
                currentStackOffset -= 8;
                maxStackUsage = std::max(maxStackUsage, -currentStackOffset / 8);
                variableOffsets[variable] = currentStackOffset;
            }

            int offset = variableOffsets[variable];
            output << "    movq " << reg << ", " << offset << "(%rbp)\n";

            state.inMemory = true;
            state.memoryOffset = offset;
            state.registerName = reg;
        }
    };

}

#endif