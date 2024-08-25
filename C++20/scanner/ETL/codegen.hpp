#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <regex>
#include "ir.hpp"
#include "symbol.hpp"

namespace codegen {
    
    enum class VariableType {
        STRING_CONST,
        NUMERIC_CONST,
        VAR,
    };

    struct VariableInfo {
        VariableType type;
        bool isAllocated;
        std::string vname; // For storing label or variable name

        VariableInfo(VariableType t, bool alloc = false, const std::string& name = "")
            : type(t), isAllocated(alloc), vname(name) {}
        VariableInfo() : type(VariableType::VAR), isAllocated(false), vname("") {} // Default constructor
    };

    class CodeEmitter {
    public:
        CodeEmitter(symbol::SymbolTable &symbolTable, std::unordered_map<std::string, int> &functionVarCount)
            : table(symbolTable), functionLocalVarCount(functionVarCount), currentStackOffset(0), maxStackUsage(0) {}

        std::string emit(const ir::IRCode &code) {
            std::ostringstream output;
            collectLiteralsAndConstants(code);
            analyzeTempVarUsage(code);
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
        std::unordered_map<std::string, std::string> numericConstants;
        std::unordered_map<std::string, std::string> valueLocations;
        std::unordered_map<std::string, int> valueToStackOffset;
        std::unordered_map<std::string, int> tempVarCountPerFunction;
        std::unordered_set<std::string> allocatedMemory;  
        std::unordered_map<std::string, VariableInfo> variableInfo;

        void collectLiteralsAndConstants(const ir::IRCode &code) {
            int const_values = 0;
            for (const auto &instr : code) {
                if (instr.type == ir::InstructionType::LOAD_CONST) {
                    if (instr.op1[0] == '\"') {
                        std::string label = "str" + std::to_string(stringLiterals.size());
                        stringLiterals[instr.op1] = label;
                        variableInfo[instr.dest] = VariableInfo(VariableType::STRING_CONST, false, label);
                    } else {
                        std::string label = "num" + std::to_string(const_values++);
                        numericConstants[instr.op1] = label;
                        variableInfo[instr.dest] = VariableInfo(VariableType::NUMERIC_CONST, false, label);      
                    }
                }
            }
        }

        void analyzeTempVarUsage(const ir::IRCode &code) {
            std::unordered_map<std::string, int> tempVarIndices;
            std::string currentFunction;

            for (const auto &instr : code) {
                if (instr.type == ir::InstructionType::LABEL) {
                    currentFunction = instr.dest;
                    tempVarCountPerFunction[currentFunction] = 0;
                    tempVarIndices.clear();
                }

                if (instr.dest[0] == 't') {
                    if (tempVarIndices.find(instr.dest) == tempVarIndices.end()) {
                        tempVarIndices[instr.dest] = tempVarCountPerFunction[currentFunction]++;
                    }
                }
            }
        }

        void emitDataSection(std::ostringstream &output) {
            output << ".section .data\n";
            for (const auto &entry : stringLiterals) {
                output << entry.second << ": .asciz " << ir::escapeString(entry.first) << "\n";
            }
            for (const auto &entry : numericConstants) {
                output << entry.second << ": .quad " << entry.first << "\n";
            }
            output << ".section .bss\n";
            output << "    .lcomm tempBufferLHS, 24\n";  // Temporary buffer for LHS conversion
            output << "    .lcomm tempBufferRHS, 24\n";  // Temporary buffer for RHS conversion
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

        void emitFunctionPrologue(std::ostringstream &output, const std::string &functionName) {
            output << "    pushq %rbp\n";
            output << "    movq %rsp, %rbp\n";

            int tempVarCount = tempVarCountPerFunction[functionName];
            int stackSpace = tempVarCount * 8;
            stackSpace = ((stackSpace + 15) / 16) * 16;

            if (stackSpace > 0) {
                output << "    subq $" << stackSpace << ", %rsp\n";
            }
        }

        void emitFunctionEpilogue(std::ostringstream &output) {
            output << "    leave\n";
            output << "    ret\n";
        }

        void emitCode(const ir::IRCode &code, std::ostringstream &output) {
            std::string currentFunction;

            for (const auto &instr : code) {
                if (instr.type == ir::InstructionType::LABEL) {
                    currentFunction = instr.dest;
                }

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
                    case ir::InstructionType::CONCAT:
                        emitConcat(output, instr);
                        break;
                    default:
                        std::cerr << "Unsupported IR Instruction: " << instr.toString() << std::endl;
                        break;
                }
            }
        }
        
        void emitLoadConst(std::ostringstream &output, const ir::IRInstruction &instr) {
            auto varType = variableInfo[instr.dest].type;
            if (varType == VariableType::STRING_CONST) {
                std::string label = stringLiterals[instr.op1];
                output << "    leaq " << label << "(%rip), %rax\n";
            } else if (varType == VariableType::NUMERIC_CONST) {
                std::string label = numericConstants[instr.op1];
                output << "    movq " << label << "(%rip), %rax\n";
            }
            storeToTemp(output, instr.dest, "%rax");
        }
        
        void emitConcat(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (variableInfo[instr.op1].type == VariableType::NUMERIC_CONST) {
                std::string numLabel = variableInfo[instr.op1].vname;
                output << "    leaq " << numLabel << "(%rip), %rdi\n";   
                output << "    leaq tempBufferLHS(%rip), %rsi\n";        
                output << "    call intToString\n"; 
                output << "    movq %rax, %rcx\n";  
            } else if (variableInfo[instr.op1].type == VariableType::STRING_CONST) {
                loadToRegister(output, instr.op1, "%rsi");
                output << "    movq %rsi, %rdi\n";  
                output << "    call strlen\n";
                output << "    movq %rax, %rcx\n";  
            } else if(variableInfo[instr.op1].type == VariableType::VAR) {
                loadToRegister(output, instr.op1, "%rsi");
                output << "    movq %rsi, %rdi\n";  
                output << "    call strlen\n";
                output << "    movq %rax, %rcx\n";  
            }

            if (variableInfo[instr.op2].type == VariableType::NUMERIC_CONST) {
                std::string numLabel = variableInfo[instr.op2].vname;
                output << "    movq " << numLabel << "(%rip), %rdi\n";   
                output << "    leaq tempBufferRHS(%rip), %rsi\n";   
                output << "    pushq %rcx\n";     
                output << "    call intToString\n";
                output << "    popq %rcx\n";                      
                output << "    addq %rax, %rcx\n";  
            } else if (variableInfo[instr.op2].type == VariableType::STRING_CONST) {
                loadToRegister(output, instr.op2, "%rdx");
                output << "    movq %rdx, %rdi\n";  
                output << "    pushq %rcx\n";
                output << "    call strlen\n";
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n";  
            } else if(variableInfo[instr.op2].type == VariableType::VAR) {
                loadToRegister(output, instr.op2, "%rdx");
                output << "    movq %rdx, %rdi\n";  
                output << "    pushq %rcx\n";
                output << "    call strlen\n";
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n";  
            }

            
            output << "    addq $1 , %rcx\n";    
            output << "    movq %rcx, %rdi\n";  
            output << "    xorq %rax, %rax\n";
            output << "    call malloc\n";
            storeToTemp(output, instr.dest, "%rax");  
            output << "    movq " << getOperand(instr.dest) << ", %rdi\n"; 
            if (variableInfo[instr.op1].type == VariableType::NUMERIC_CONST) {
                output << "    leaq tempBufferLHS(%rip), %rsi\n";              
            } else {
                output << "    movq " << getOperand(instr.op1) << ", %rsi\n";  
            }
            output << "    call strcpy\n";
            output << "    movq " << getOperand(instr.dest) << ", %rdi\n"; 
            if (variableInfo[instr.op2].type == VariableType::NUMERIC_CONST) {
                output << "    leaq tempBufferRHS(%rip), %rsi\n";              
            } else {
                output << "    movq " << getOperand(instr.op2) << ", %rsi\n";  
            }
            output << "    call strcat\n";
            output << "    movq " << getOperand(instr.dest) << ", %rax\n"; 
            allocatedMemory.insert(instr.dest);  
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
            if (instr.dest[0] == 't') {
                return; 
            }

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
            output << "    movq $0, %rax\n"; 
            output << "    call " << instr.functionName << "\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitLabel(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << instr.dest << ":\n";
            if (instr.dest != "main") {
                emitFunctionPrologue(output, instr.dest);
            }
        }

        void emitReturn(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (!instr.dest.empty()) {
                loadToRegister(output, instr.dest, "%rax");
            } else {
                output << "    movq $0, %rax\n";
            }

            for (const auto &var : allocatedMemory) {
                output << "    movq " << getOperand(var) << ", %rdi\n";
                output << "    call free\n";
            }

            emitFunctionEpilogue(output);
        }

        void loadToRegister(std::ostringstream &output, const std::string &operand, const std::string &reg) {
            if (operand[0] == '$' || operand[0] == '%') {
                output << "    movq " << operand << ", " << reg << "\n";
            } else if(operand[0] == isalpha(operand[0]) || isdigit(operand[0])) {
                output << "    leaq " << operand << "%(rip), " << reg << "\n";
            } 
            else {
                int offset = getVariableOffset(operand);
                output << "    movq " << offset << "(%rbp), " << reg << "\n";
            }
        }

        void storeToTemp(std::ostringstream &output, const std::string &temp, const std::string &reg) {
            if (valueLocations[temp] == reg) {
                return; 
            }
            int offset = getVariableOffset(temp);
            valueToStackOffset[temp] = offset;
            output << "    movq " << reg << ", " << offset << "(%rbp)\n";
            valueLocations[temp] = reg;
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

        std::string optimizeRedundantLoadStore(const std::string &assemblyCode) {
            std::istringstream input(assemblyCode);
            std::ostringstream output;
            std::string line;
            std::string lastStoreLine;
            std::string lastStoreLocation;
            std::string lastStoreRegister;
            bool lastWasStore = false;

            std::regex movqRegex(R"(movq\s+([^\s,]+)\s*,\s*([^\s]+))");

            while (std::getline(input, line)) {
                std::smatch match;
                if (std::regex_search(line, match, movqRegex)) {
                    std::string src = match[1];
                    std::string dest = match[2];

                    if (lastWasStore) {
                        if (src == lastStoreLocation && dest == lastStoreRegister) {
                            lastWasStore = false; 
                            continue;
                        }
                    }

                    if (dest.find("(%rbp)") != std::string::npos) {
                        lastStoreLine = line;
                        lastStoreLocation = dest;
                        lastStoreRegister = src;
                        lastWasStore = true;
                    } else {
                        lastWasStore = false;
                    }
                } else {
                    lastWasStore = false;
                }

                output << line << "\n";
            }
            return output.str();
        }

        std::string applyPeephole(std::ostringstream &output) {
            return optimizeRedundantLoadStore(output.str());
        }
    };

}

#endif