#ifndef _IR_X_H_
#define _IR_X_H_

#include<string>
#include<vector>
#include<iostream> 

namespace ir {
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
        CALL
    };

    inline std::vector<std::string> InstructionStrings {
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "ASSIGN",
        "LOAD_CONST",
        "LOAD_VAR",
        "LABEL",
        "NEG",
        "CALL"
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
            if (!op1.empty()) result += " " + op1;
            if (!op2.empty()) result += " " + op2;
            for (const auto& arg : args) {
                result += " " + arg;
            }
            return result;
        }
    };

    using IRCode = std::vector<IRInstruction>;
}

namespace parse {

    class IRGenerator {
    public:
        ir::IRCode generateIR(const std::unique_ptr<ast::ASTNode> &ast) {
            ir::IRCode code;
            generate(ast.get(), code);
            return code;
        }

    private:
        int tempVarCounter = 0;  

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
            }
        }

        void generateProgram(const ast::Program *program, ir::IRCode &code) {
            for (const auto &stmt : program->body) {
                generate(stmt.get(), code);
            }
        }

        void generateAssignment(const ast::Assignment *assign, ir::IRCode &code) {
            generate(assign->right.get(), code);
            std::string rhs = "t" + std::to_string(tempVarCounter - 1);  
            auto lhs = dynamic_cast<const ast::Identifier*>(assign->left.get());
            if (lhs) {
                code.emplace_back(ir::InstructionType::ASSIGN, lhs->name, rhs);
            } else {
                std::cerr << "Error: LHS of assignment is not an identifier\n";
                exit(EXIT_FAILURE);
            }
        }

        void generateBinaryOp(const ast::BinaryOp *binOp, ir::IRCode &code) {
            generate(binOp->left.get(), code);
            std::string leftResult = "t" + std::to_string(tempVarCounter - 1);  
            generate(binOp->right.get(), code);
            std::string rightResult = "t" + std::to_string(tempVarCounter - 1);  

            std::string dest = "t" + std::to_string(tempVarCounter++);  

            switch (binOp->op) {
                case types::OperatorType::OP_PLUS:
                    code.emplace_back(ir::InstructionType::ADD, dest, leftResult, rightResult);
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
                    std::cerr << "Error: Unsupported operator in binary operation\n";
                    exit(EXIT_FAILURE);
            }
        }

        void generateUnaryOp(const ast::UnaryOp *unaryOp, ir::IRCode &code) {
            generate(unaryOp->operand.get(), code);
            std::string result = "t" + std::to_string(tempVarCounter - 1);  // Latest tempVar

            if (unaryOp->op == types::OperatorType::OP_MINUS) {
                code.emplace_back(ir::InstructionType::NEG, result, result);
            } else {
                std::cerr << "Error: Unsupported unary operator\n";
                exit(EXIT_FAILURE);
            }
        }

        void generateFunction(const ast::Function *func, ir::IRCode &code) {
            code.emplace_back(ir::InstructionType::LABEL, func->name, "");
            for (const auto &stmt : func->body) {
                generate(stmt.get(), code);
            }
        }

        void generateLiteral(const ast::Literal *literal, ir::IRCode &code) {
            std::string tempVar = "t" + std::to_string(tempVarCounter++);
            code.emplace_back(ir::InstructionType::LOAD_CONST, tempVar, literal->value);
        }

        void generateIdentifier(const ast::Identifier *identifier, ir::IRCode &code) {
            std::string tempVar = "t" + std::to_string(tempVarCounter++);
            code.emplace_back(ir::InstructionType::LOAD_VAR, tempVar, identifier->name);
        }

        void generateCall(const ast::Call *call, ir::IRCode &code) {
            std::vector<std::string> argRegisters;
            for (const auto &arg : call->arguments) {
                generate(arg.get(), code);  

                std::string tempVar = "t" + std::to_string(tempVarCounter - 1);  
                argRegisters.push_back(tempVar);  
            }
            std::string callDest = "t" + std::to_string(tempVarCounter++);  
            code.emplace_back(ir::InstructionType::CALL, callDest, call->functionName, argRegisters);
        }
    };
}

#endif