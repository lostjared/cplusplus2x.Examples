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
#include "clib.hpp"

namespace codegen {
    
    enum class ReturnType {
        INTEGER,
        POINTER,
        VOID, 
        FLOAT,
        DOUBLE,
    };

    inline std::unordered_map<std::string, ReturnType> funcReturn = {
        {"malloc", ReturnType::POINTER},
        {"free", ReturnType::VOID},
        {"strlen", ReturnType::INTEGER},
        {"printf", ReturnType::INTEGER}, 
        {"atoi", ReturnType::INTEGER},
        {"str", ReturnType::POINTER},
    };

    enum class VariableType {
        STRING_CONST,
        NUMERIC_CONST,
        VAR,
        VAR_STRING,
    };

    struct VariableInfo {
        VariableType type;
        bool isAllocated;
        std::string vname, text;
        VariableInfo(VariableType t, bool alloc = false, const std::string& name = "", const std::string &txt = "")
            : type(t), isAllocated(alloc), vname(name), text(txt) {}
        VariableInfo() : type(VariableType::VAR), isAllocated(false), vname(""), text("") {} 
    };

    class CodeEmitter {
    public:
        CodeEmitter(symbol::SymbolTable &symbolTable, std::unordered_map<std::string, int> &functionVarCount)
            : table(symbolTable), functionLocalVarCount(functionVarCount), currentStackOffset{}, maxStackUsage{} {}

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
        symbol::SymbolTable local;
        std::unordered_map<std::string, int> &functionLocalVarCount;
        std::unordered_map<std::string, int> currentStackOffset;
        std::unordered_map<std::string, int> maxStackUsage;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> variableOffsets;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> stringLiterals;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> numericConstants;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> valueLocations;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> valueToStackOffset;
        std::unordered_map<std::string, int> tempVarCountPerFunction;
        std::unordered_map<std::string, std::unordered_set<std::string>> allocatedMemory;  
        std::unordered_map<std::string, std::unordered_map<std::string, VariableInfo>> variableInfo;
        std::string curFunction;

        void collectLiteralsAndConstants(const ir::IRCode &code) {
            for (const auto &instr : code) {

                if(instr.type == ir::InstructionType::LABEL) {
                    curFunction = instr.dest;
                }

                if (instr.type == ir::InstructionType::LOAD_CONST) {
                    if (instr.op1[0] == '\"') {
                        std::string label =  instr.dest;
                        stringLiterals[curFunction][instr.op1] = label;
                        variableInfo[curFunction][instr.dest] = VariableInfo(VariableType::STRING_CONST, false, label, instr.op1);
                        local.enter(label);
                        auto it = local.lookup(label);
                        if(it.has_value()) {
                            symbol::Symbol *s = it.value();
                            s->name = label;
                            s->value = instr.op1;
                        }
                    } else {
                        std::string label = instr.dest;
                        if(numericConstants[curFunction].find(instr.op1) == numericConstants[curFunction].end()) {
                            numericConstants[curFunction][instr.op1] = label;
                            variableInfo[curFunction][instr.dest] = VariableInfo(VariableType::NUMERIC_CONST, false, label, instr.op1);
                            local.enter(label);
                            auto it = local.lookup(label);
                            if(it.has_value()) {
                                symbol::Symbol *s = it.value();
                                 s->name = label;
                                s->value = instr.op1;
                            }
                        }
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
            for(auto &func : variableInfo) {
                for(const auto &v : func.second) {
                    if(v.second.type == VariableType::NUMERIC_CONST) {
                        output << v.second.vname << ": .quad " << v.second.text << "\n";
                    } else if(v.second.type == VariableType::STRING_CONST) {
                        output << v.second.vname << ": .asciz " << ir::escapeString(v.second.text) << "\n";
                    }
            }
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
                    case ir::InstructionType::NOT:
                        emitNot(output, instr);
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
            if (instr.op1[0] == '\"') {
                std::string label = stringLiterals[curFunction][instr.op1];
                output << "    leaq " << label << "(%rip), %rax\n";
                variableInfo[curFunction][instr.op1].type == VariableType::STRING_CONST;
            } else {
                std::string label = numericConstants[curFunction][instr.op1];
                output << "    movq " << "$" << instr.op1 << ", %rax\n";
                variableInfo[curFunction][instr.op1].type == VariableType::NUMERIC_CONST;
            } 
            storeToTemp(output, instr.dest, "%rax");
        }
        
        void emitConcat(std::ostringstream &output, const ir::IRInstruction &instr) {

            output << "    xorq %rcx, %rcx\n";

            if (variableInfo[curFunction][instr.op1].type == VariableType::STRING_CONST) {
                if(!variableInfo[curFunction][instr.op1].text.empty()  && variableInfo[curFunction][instr.op1].text[0] == '\"') {
                    auto len = variableInfo[curFunction][instr.op1].text.length()+1;
                    output << "     addq $" << len << ", %rcx\n";
                } else {
                    loadToRegister(output, instr.op1, "%rdi");
                    output << "     call strlen\n";
                    output << "     addq  %rax, %rcx\n";
                }
            } else if(variableInfo[curFunction][instr.op1].type == VariableType::NUMERIC_CONST) {
                loadToRegister(output, instr.op1 , "%rdi");
                output << "    leaq tempBufferLHS(%rip), %rsi\n"; 
                output << "    pushq %rcx\n";
                output << "    call intToString\n"; 
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n"; 
                output << "    movq %rsi, " << getOperand(instr.op1) << "\n";
            } else if(variableInfo[curFunction][instr.op1].type == VariableType::VAR_STRING) {
                loadToRegister(output, instr.op1, "%rdi");
                output << "    call strlen\n";
                output << "    addq %rax, %rcx\n";
            } else if(variableInfo[curFunction][instr.op1].type == VariableType::VAR) {
                loadToRegister(output, instr.op1 , "%rdi");
                output << "    leaq tempBufferRHS(%rip), %rsi\n";
                output << "    pushq %rcx\n"; 
                output << "    call intToString\n"; 
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n";
            }

            if (variableInfo[curFunction][instr.op2].type == VariableType::STRING_CONST) {
                if(!variableInfo[curFunction][instr.op2].text.empty()  && variableInfo[curFunction][instr.op2].text[0] == '\"') {
                    auto len = variableInfo[curFunction][instr.op1].text.length()+1;
                    output << "    addq $" << len << ", %rcx\n";
                } else {
                    loadToRegister(output, instr.op2, "%rdi");
                    output << "    call strlen\n";
                    output << "    addq %rax, %rcx\n";
                }
            } else if(variableInfo[curFunction][instr.op2].type == VariableType::NUMERIC_CONST) {
                loadToRegister(output, instr.op2 , "%rdi");
                output << "    leaq tempBufferRHS(%rip), %rsi\n";
                output << "    pushq %rcx\n"; 
                output << "    call intToString\n"; 
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n"; 
                
            } else if (variableInfo[curFunction][instr.op2].type == VariableType::VAR_STRING) {
                loadToRegister(output, instr.op2, "%rdi");
                output << "    call strlen\n";
                output << "    addq %rax, %rcx\n";
            } else if(variableInfo[curFunction][instr.op2].type == VariableType::VAR) {
                loadToRegister(output, instr.op2 , "%rdi");
                output << "    leaq tempBufferRHS(%rip), %rsi\n";
                output << "    pushq %rcx\n"; 
                output << "    call intToString\n"; 
                output << "    popq %rcx\n";
                output << "    addq %rax, %rcx\n"; 
            }
            output << "    addq $1, %rcx\n";
            output << "    movq %rcx, %rdi\n";
            output << "    xorq %rax, %rax\n";
            output << "    call malloc\n";
            storeToTemp(output, instr.dest, "%rax");
            output << "    movq %rax, %rdi\n";
            if(variableInfo[curFunction][instr.op1].type == VariableType::NUMERIC_CONST || variableInfo[curFunction][instr.op1].type == VariableType::VAR) {
                output << "    leaq tempBufferLHS(%rip), %rsi\n"; 
            } else  {
                 loadToRegister(output,instr.op1,"%rsi");
            }
            output << "    call strcpy\n";
            if(variableInfo[curFunction][instr.op2].type == VariableType::NUMERIC_CONST || variableInfo[curFunction][instr.op2].type == VariableType::VAR) {
                output << "    leaq tempBufferRHS(%rip), %rsi\n";
            } else {
                loadToRegister(output,instr.op2,"%rsi");
            }
            output << "    call strcat\n";
            allocatedMemory[curFunction].insert(instr.dest);  
            variableInfo[curFunction][instr.dest].type = VariableType::VAR_STRING;
        }

        void emitBinaryOp(std::ostringstream &output, const ir::IRInstruction &instr, const std::string &op) {
            variableInfo[curFunction][instr.dest].type = VariableType::VAR;
            loadToRegister(output, instr.op1, "%rax");
            output << "    " << op << " " << getOperand(instr.op2) << ", %rax\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitDiv(std::ostringstream &output, const ir::IRInstruction &instr) {
            variableInfo[curFunction][instr.dest].type = VariableType::VAR;
            loadToRegister(output, instr.op1, "%rax");
            output << "    cqto\n";
            output << "    idivq " << getOperand(instr.op2) << "\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitAssign(std::ostringstream &output, const ir::IRInstruction &instr) {
            variableInfo[curFunction][instr.dest].type = variableInfo[curFunction][instr.op1].type;
            loadToRegister(output, instr.op1, "%rax");
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitLoadVar(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            storeToTemp(output, instr.dest, "%rax");
            variableInfo[curFunction][instr.dest].type = variableInfo[curFunction][instr.op1].type;
        }

        void emitNeg(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    negq %rax\n";
            storeToTemp(output, instr.dest, "%rax");
        }

        void emitNot(std::ostringstream &output, const ir::IRInstruction &instr) {
            loadToRegister(output, instr.op1, "%rax");
            output << "    notq %rax\n";
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
            
            auto fn = clib::clibrary.find(instr.functionName);
            
            if (fn != clib::clibrary.end()) {
                switch (fn->second.return_type) {
                    case clib::VarType::POINTER:
                        variableInfo[curFunction][instr.dest].type = VariableType::VAR_STRING;
                        break;
                    case clib::VarType::INTEGER:
                        variableInfo[curFunction][instr.dest].type = VariableType::VAR;
                        break;
                    default:
                        std::cerr << "ETL: Return type not supported yet.\n";
                        exit(EXIT_FAILURE);
                        break;
                }

                if (instr.args.size() != fn->second.args.size() && instr.functionName != "printf") {
                    std::cerr << "ETL: Fatal, incorrect number of arguments for: " << instr.functionName << "\n";
                    exit(EXIT_FAILURE);
                }

                if (instr.functionName != "printf") {
                    for (size_t i = 0; i < fn->second.args.size(); ++i) {
                        auto actualType = variableInfo[curFunction][instr.args.at(i)].type;
                        auto expectedType = fn->second.args[i];

                        clib::VarType actualVarType;
                        switch (actualType) {
                            case VariableType::VAR_STRING: actualVarType = clib::VarType::POINTER; break;
                            case VariableType::VAR:        actualVarType = clib::VarType::INTEGER; break;
                            case VariableType::STRING_CONST: actualVarType = clib::VarType::POINTER; break;
                            case VariableType::NUMERIC_CONST: actualVarType = clib::VarType::INTEGER; break;
                            default:
                                std::cerr << "ETL: Unsupported variable type for: " << instr.args.at(i) << " argument.\n";
                                exit(EXIT_FAILURE);
                        }

                        if (actualVarType != expectedType) {
                            std::cerr << "ETL: Type mismatch for argument " << i << " in function " << instr.functionName << "\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                }

                if (fn->second.allocated) {
                    allocatedMemory[curFunction].insert(instr.dest);
                }
            }
        }

        void emitLabel(std::ostringstream &output, const ir::IRInstruction &instr) {
            output << instr.dest << ":\n";
            curFunction = instr.dest;
            if (instr.dest != "main") {
                emitFunctionPrologue(output, instr.dest);
            }
            local.enterScope(curFunction);
        }

        void emitReturn(std::ostringstream &output, const ir::IRInstruction &instr) {
            if (!instr.dest.empty()) {
                loadToRegister(output, instr.dest, "%rax");
            } else {
                output << "    movq $0, %rax\n";
            }

            for (const auto &var : allocatedMemory[curFunction]) {
                output << "    movq " << getOperand(var) << ", %rdi\n";
                output << "    call free\n";
            }
            emitFunctionEpilogue(output);
            local.exitScope();
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
            if (valueLocations[curFunction][temp] == reg) {
                return; 
            }
            int offset = getVariableOffset(temp);
            valueToStackOffset[curFunction][temp] = offset;
            output << "    movq " << reg << ", " << offset << "(%rbp)\n";
            valueLocations[curFunction][temp] = reg;
        }

        int getVariableOffset(const std::string &varName) {
            if (variableOffsets[curFunction].find(varName) == variableOffsets[curFunction].end()) {
                currentStackOffset[curFunction] -= 8;
                variableOffsets[curFunction][varName] = currentStackOffset[curFunction];
                maxStackUsage[curFunction] = std::min(maxStackUsage[curFunction], currentStackOffset[curFunction]);
            }
            return variableOffsets[curFunction][varName];
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