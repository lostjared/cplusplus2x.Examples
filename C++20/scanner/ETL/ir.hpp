#ifndef _IR_X_H_
#define _IR_X_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "symbol.hpp"
#include "ast.hpp"

namespace ir {

    std::string escapeString(const std::string &input);

    enum class InstructionType {
        ADD,
        SUB,
        MUL,
        DIV,
        ASSIGN,
        LOAD_CONST,
        LOAD_VAR,
        LABEL,
        NOT,
        NEG,
        CALL,
        RETURN,
        CONCAT
    };

    inline std::vector<std::string> InstructionStrings{
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "ASSIGN",
        "LOAD_CONST",
        "LOAD_VAR",
        "LABEL",
        "NOT",
        "NEG",
        "CALL",
        "RETURN",
        "CONCAT"
    };

    struct IRInstruction {
        InstructionType type;
        std::string dest;
        std::string op1;
        std::string op2;
        std::vector<std::string> args;
        std::string functionName;

        IRInstruction(InstructionType t, const std::string &d, const std::string &o1 = "", const std::string &o2 = "")
            : type(t), dest(d), op1(o1), op2(o2) {}

        IRInstruction(InstructionType t, const std::string &d, const std::string &funcName, const std::vector<std::string> &a)
            : type(t), dest(d), functionName(funcName), args(a) {}

        std::string toString() const {
            std::string result = InstructionStrings[static_cast<int>(type)] + " " + dest;
            if (!functionName.empty()) result += " " + functionName;
            if (!op1.empty()) result += " " + escapeString(op1);
            if (!op2.empty()) result += " " + escapeString(op2);
            for (const auto &arg : args) {
                result += " " + arg;
            }
            return result;
        }
    };

    using IRCode = std::vector<IRInstruction>;

    struct IRContext {
        IRCode instructions;
        symbol::SymbolTable table;
        std::unordered_map<std::string, int> functionLocalVarCount;

        IRContext() : instructions(), table(), functionLocalVarCount() {}
    };

    class IRException {
    public:
        IRException(const std::string &what) : why_{what} {}
        std::string why() const { return why_; }
    private:
        std::string why_;
    };

}

namespace parse {

    class IRGenerator {
    public:
        symbol::SymbolTable table;
        std::unordered_map<std::string, int> functionLocalVarCount;

        ir::IRContext generateIR(const std::unique_ptr<ast::ASTNode> &ast) {
            ir::IRContext context;
            generate(ast.get(), context.instructions);
            context.table = table;
            context.functionLocalVarCount = functionLocalVarCount;
            return context;
        }

    private:
        int tempVarCounter = 0;

        std::string getNextTempVar() {
            return "t" + std::to_string(tempVarCounter++);
        }

        void generate(const ast::ASTNode *node, ir::IRCode &code) {
            if (auto program = dynamic_cast<const ast::Program*>(node)) {
                generateProgram(program, code);
            } else if (auto assign = dynamic_cast<const ast::Assignment*>(node)) {
                generateAssignment(assign, code);
            } else if (auto binOp = dynamic_cast<const ast::BinaryOp*>(node)) {
                generateBinaryOp(binOp, code);
            } else if (auto unaryOp = dynamic_cast<const ast::UnaryOp*>(node)) {
                generateUnaryOp(unaryOp, code);
            } else if (auto func = dynamic_cast<const ast::Function*>(node)) {
                generateFunction(func, code);
            } else if (auto call = dynamic_cast<const ast::Call*>(node)) {
                generateCall(call, code);
            } else if (auto literal = dynamic_cast<const ast::Literal*>(node)) {
                generateLiteral(literal, code);
            } else if (auto identifier = dynamic_cast<const ast::Identifier*>(node)) {
                generateIdentifier(identifier, code);
            } else if (auto rt = dynamic_cast<const ast::Return*>(node)) {
                generateReturn(rt, code);
            }
        }

        void generateProgram(const ast::Program *program, ir::IRCode &code) {
            for (const auto &stmt : program->body) {
                generate(stmt.get(), code);
            }
        }

        void generateAssignment(const ast::Assignment *assign, ir::IRCode &code) {
            auto lhs = dynamic_cast<const ast::Identifier*>(assign->left.get());
            if (lhs) {
                auto rhsLiteral = dynamic_cast<const ast::Literal*>(assign->right.get());
                if (rhsLiteral) {
                    table.enter(lhs->name);
                    auto entry = table.lookup(lhs->name);
                    if(entry.has_value()) {
                        symbol::Symbol *e = entry.value();
                        e->name = lhs->name;
                        e->value = rhsLiteral->value;      
                        if(rhsLiteral->type == types::TokenType::TT_NUM)
                            e->vtype = ast::VarType::NUMBER;
                        else if(rhsLiteral->type == types::TokenType::TT_STR)
                            e->vtype = ast::VarType::STRING;
                    }
                    
                    code.emplace_back(ir::InstructionType::LOAD_CONST, lhs->name, rhsLiteral->value);
                } else {
                    generate(assign->right.get(), code);
                    std::string rhs = lastComputedValue["result"];
                    if (lastComputedValue[rhs] == lhs->name) {
                        lastComputedValue[lhs->name] = rhs;
                    } else {
                        table.enter(lhs->name);
                        auto it = table.lookup(lhs->name);
                        if(it.has_value()) {
                            symbol::Symbol *s = it.value();
                            table.enter(rhs);
                            auto r = table.lookup(rhs);
                            if(r.has_value()) {
                                s->vtype = r.value()->vtype;
                            }
                        }
                        code.emplace_back(ir::InstructionType::ASSIGN, lhs->name, rhs);

                        lastComputedValue[lhs->name] = lhs->name;
                    }
                }
            } else {
                throw ir::IRException("IR Fatal Error: LHS of assignment is not an identifier");
            }
        }

        void generateReturn(const ast::Return *return_value, ir::IRCode &code) {
            if (return_value->return_value) {
                generate(return_value->return_value.get(), code);
                std::string result = lastComputedValue["result"];
                code.emplace_back(ir::InstructionType::RETURN, result);
            } else {
                code.emplace_back(ir::InstructionType::RETURN, "");
            }
        }

        void generateBinaryOp(const ast::BinaryOp *binOp, ir::IRCode &code) {
            generate(binOp->left.get(), code);
            std::string leftResult = lastComputedValue["result"];
            generate(binOp->right.get(), code);
            std::string rightResult = lastComputedValue["result"];

            std::string dest = getNextTempVar();

            bool leftIsString = false;
            bool rightIsString = false;

            if (auto identifier = dynamic_cast<ast::Identifier*>(binOp->left.get())) {
                    leftIsString = identifier->vtype == ast::VarType::STRING;
            }


            if (auto identifier = dynamic_cast<ast::Identifier*>(binOp->right.get())) {
                    rightIsString = identifier->vtype == ast::VarType::STRING;
            }

            if(!leftIsString) {
                auto it = table.lookup(leftResult);
                if(it.has_value()) {
                    symbol::Symbol *s = it.value();
                    if((!s->value.empty() && s->value[0] == '\"') || s->vtype == ast::VarType::STRING)
                        leftIsString = true;
                }
            }

            
            if(!rightIsString) {
                auto it = table.lookup(rightResult);
                if(it.has_value()) {
                    symbol::Symbol *s = it.value();
                    if((!s->value.empty() && s->value[0] == '\"') || s->vtype == ast::VarType::STRING)
                        rightIsString = true;
                }
            }
     
            if (leftIsString && rightIsString) {
                code.emplace_back(ir::InstructionType::CONCAT, dest, leftResult, rightResult);
                table.enter(dest);
                auto it = table.lookup(dest);
                if (it.has_value()) {
                    it.value()->name = dest;
                    it.value()->value = '\"';
                    it.value()->vtype = ast::VarType::STRING;
                }
            } else if (!leftIsString && !rightIsString) {
                    table.enter(dest);
                    auto it = table.lookup(dest);
                    if(it.has_value()) {
                        symbol::Symbol *s = it.value();
                        s->name = dest;
                        s->vtype = ast::VarType::NUMBER;
                    }
                    code.emplace_back(ir::InstructionType::ADD, dest, leftResult, rightResult);

            } else {
                std::ostringstream stream;
                stream << "Error: Operator + requires both operands to be of string type or both to be of number type. "
                    << "Found: " << (leftIsString ? "string" : "number") << " + " 
                    << (rightIsString ? "string" : "number") << ". Use str() to convert.";
                throw ir::IRException(stream.str());
            }

            lastComputedValue["result"] = dest;
        }
        void generateUnaryOp(const ast::UnaryOp *unaryOp, ir::IRCode &code) {
            generate(unaryOp->operand.get(), code);
            std::string result = lastComputedValue["result"];

            std::string dest = getNextTempVar();
            if(unaryOp->op == types::OperatorType::OP_TILDE)
                code.emplace_back(ir::InstructionType::NOT, dest, result);
            else
                code.emplace_back(ir::InstructionType::NEG, dest, result);
               
            lastComputedValue["result"] = dest;
        }

        void generateFunction(const ast::Function *func, ir::IRCode &code) {
            code.emplace_back(ir::InstructionType::LABEL, func->name, "");
            table.enterScope(func->name);
            for (const auto &stmt : func->body) {
                generate(stmt.get(), code);
            }
            functionLocalVarCount[func->name] = table.getCurrentScopeSize();
            table.exitScope();
        }

        void generateLiteral(const ast::Literal *literal, ir::IRCode &code) {
            std::string tempVar = getNextTempVar();
            code.emplace_back(ir::InstructionType::LOAD_CONST, tempVar, literal->value);
            table.enter(tempVar);
            auto value = table.lookup(tempVar);
            if(value.has_value()) {
                symbol::Symbol *v = value.value();
                v->value = literal->value;
                v->name = tempVar;
                if(literal->value.empty() && literal->value[0] == '\"') {
                    v->vtype = ast::VarType::STRING;
                } else {
                    v->vtype = ast::VarType::NUMBER;
                }
            }
            lastComputedValue["result"] = tempVar;
        }

        void generateIdentifier(const ast::Identifier *identifier, ir::IRCode &code) {
            if (lastComputedValue.find(identifier->name) != lastComputedValue.end()) {
                lastComputedValue["result"] = lastComputedValue[identifier->name];
            } else {
                std::string tempVar = getNextTempVar();

                table.enter(tempVar);
                auto v = table.lookup(tempVar);
                if(v.has_value()) {
                    symbol::Symbol *vx = v.value();
                    vx->name = tempVar;
                    vx->vtype = identifier->vtype;
                    auto cpx = table.lookup(identifier->name);
                    if(cpx.has_value()) {
                        vx->value = cpx.value()->value;
                        vx->vtype = cpx.value()->vtype;
                    }
                }
                code.emplace_back(ir::InstructionType::LOAD_VAR, tempVar, identifier->name);
                lastComputedValue["result"] = tempVar;
            }
        }

        void generateCall(const ast::Call *call, ir::IRCode &code) {
            std::vector<std::string> argRegisters;
            for (const auto &arg : call->arguments) {
                generate(arg.get(), code);
                argRegisters.push_back(lastComputedValue["result"]);
            }
            std::string callDest = getNextTempVar();
            table.enter(callDest);
            auto e = table.lookup(callDest);
            if(e.has_value()) {
                symbol::Symbol *s = e.value();
                s->name = callDest;
                if(call->functionName == "str") {
                    s->vtype = ast::VarType::STRING;
                }
            }
            code.emplace_back(ir::InstructionType::CALL, callDest, call->functionName, argRegisters);
            lastComputedValue["result"] = callDest;
        }

        std::unordered_map<std::string, std::string> lastComputedValue;
    };

}

#endif