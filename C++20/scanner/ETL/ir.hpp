#ifndef _IR_X_H_
#define _IR_X_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include "symbol.hpp"

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
            peepholeOptimize(context.instructions);
            return context;
        }

    private:
        int tempVarCounter = 0;

        std::string getTempVar() {
            return "t" + std::to_string(tempVarCounter++);
        }

        void peepholeOptimize(ir::IRCode &code) {
            if (code.empty()) return;

            ir::IRCode optimizedCode;
            ir::IRInstruction lastInstruction(ir::InstructionType::LABEL, "");

            for (size_t i = 0; i < code.size(); ++i) {
                ir::IRInstruction &instr = code[i];
                if (i > 0 && instr.type == ir::InstructionType::LOAD_VAR && lastInstruction.type == ir::InstructionType::ASSIGN) {
                    if (instr.op1 == lastInstruction.dest && instr.dest == lastInstruction.op1) {
                        continue;
                    }
                }
                optimizedCode.push_back(instr);
                lastInstruction = instr;
            }
            code = std::move(optimizedCode);
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
            generate(assign->right.get(), code);
            std::string rhs = "t" + std::to_string(tempVarCounter - 1);  // Using a new temp var

            auto lhs = dynamic_cast<const ast::Identifier*>(assign->left.get());
            if (lhs) {
                code.emplace_back(ir::InstructionType::ASSIGN, lhs->name, rhs);
                updateLocalVariableCount(lhs->name);
            } else {
                throw ir::IRException("IR Fatal Error: LHS of assignment is not an identifier");
            }
        }

        void generateReturn(const ast::Return *return_value, ir::IRCode &code) {
            if (return_value->return_value) {
                generate(return_value->return_value.get(), code);
                std::string result = "t" + std::to_string(tempVarCounter - 1);  // Using a new temp var
                code.emplace_back(ir::InstructionType::RETURN, result);
            } else {
                code.emplace_back(ir::InstructionType::RETURN, "");
            }
        }

        void generateBinaryOp(const ast::BinaryOp *binOp, ir::IRCode &code) {
            generate(binOp->left.get(), code);
            std::string leftResult = "t" + std::to_string(tempVarCounter - 1);  // Using a new temp var

            generate(binOp->right.get(), code);
            std::string rightResult = "t" + std::to_string(tempVarCounter - 1);  // Using a new temp var

            std::string dest = getTempVar();

            const ast::Literal *leftLiteral = dynamic_cast<const ast::Literal *>(binOp->left.get());
            const ast::Literal *rightLiteral = dynamic_cast<const ast::Literal *>(binOp->right.get());
            bool isStringOperation = (leftLiteral && leftLiteral->type == types::TokenType::TT_STR) ||
                                     (rightLiteral && rightLiteral->type == types::TokenType::TT_STR);

            switch (binOp->op) {
                case types::OperatorType::OP_PLUS:
                    if (isStringOperation) {
                        code.emplace_back(ir::InstructionType::CONCAT, dest, leftResult, rightResult);
                    } else {
                        code.emplace_back(ir::InstructionType::ADD, dest, leftResult, rightResult);
                    }
                    break;
                case types::OperatorType::OP_MINUS:
                    code.emplace_back(ir::InstructionType::SUB, dest, leftResult, rightResult);
                    break;
                case types::OperatorType::OP_MUL:
                    code.emplace_back(ir::InstructionType::MUL, dest, leftResult, rightResult);
                    break;
                case types::OperatorType::OP_DIV:
                    code.emplace_back(ir::InstructionType::DIV, dest, leftResult, rightResult);
                    break;
                default:
                    throw ir::IRException("Error: Unsupported operator in binary operation");
            }

            table.enter(dest);
        }

        void generateUnaryOp(const ast::UnaryOp *unaryOp, ir::IRCode &code) {
            generate(unaryOp->operand.get(), code);
            std::string result = "t" + std::to_string(tempVarCounter - 1);  // Using a new temp var

            std::string dest = getTempVar();
            code.emplace_back(ir::InstructionType::NEG, dest, result);
            table.enter(dest);
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
            std::string tempVar = getTempVar();
            code.emplace_back(ir::InstructionType::LOAD_CONST, tempVar, literal->value);
            table.enter(tempVar);
        }

        void generateIdentifier(const ast::Identifier *identifier, ir::IRCode &code) {
            std::string tempVar = getTempVar();
            code.emplace_back(ir::InstructionType::LOAD_VAR, tempVar, identifier->name);
            table.enter(tempVar);
        }

        void generateCall(const ast::Call *call, ir::IRCode &code) {
            std::vector<std::string> argRegisters;
            for (const auto &arg : call->arguments) {
                generate(arg.get(), code);
                argRegisters.push_back("t" + std::to_string(tempVarCounter - 1));  // Using a new temp var
            }
            std::string callDest = getTempVar();
            code.emplace_back(ir::InstructionType::CALL, callDest, call->functionName, argRegisters);
        }

        void updateLocalVariableCount(const std::string &varName) {
            if (table.lookup(varName).has_value()) {
                table.enter(varName);
            }
        }
    };

}

#endif