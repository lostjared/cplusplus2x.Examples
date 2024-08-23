#ifndef _IR_X_H_
#define _IR_X_H_

#include<string>
#include<vector>

namespace ir {
    enum class InstructionType {
        ADD,         // Addition: t0 = t1 + t2
        SUB,         // Subtraction: t0 = t1 - t2
        MUL,         // Multiplication: t0 = t1 * t2
        DIV,         // Division: t0 = t1 / t2
        ASSIGN,      // Assignment: var = t0
        LOAD_CONST,  // Load constant into a temporary variable: t0 = const
        LOAD_VAR,    // Load variable value into a temporary variable: t0 = var
        LABEL,       // Function or block label: label:
        NEG,         // NEGATE
        // Add more instruction types as needed
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
        "NEG"
    };

    struct IRInstruction {
        InstructionType type;
        std::string dest;
        std::string op1;
        std::string op2;

        IRInstruction(InstructionType t, const std::string &d, const std::string &o1 = "", const std::string &o2 = "")
            : type(t), dest(d), op1(o1), op2(o2) {}
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
            std::string rhs = "t0";  
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
            generate(binOp->right.get(), code);

            std::string dest = "t0";  
            std::string op1 = "t0";   
            std::string op2 = "t1";   

            switch (binOp->op) {
                case types::OperatorType::OP_PLUS:
                    code.emplace_back(ir::InstructionType::ADD, dest, op1, op2);
                    break;
                case types::OperatorType::OP_MINUS:
                    code.emplace_back(ir::InstructionType::SUB, dest, op1, op2);
                    break;
                case types::OperatorType::OP_MUL:
                    code.emplace_back(ir::InstructionType::MUL, dest, op1, op2);
                    break;
                case types::OperatorType::OP_DIV:
                    code.emplace_back(ir::InstructionType::DIV, dest, op1, op2);
                    break;
                
             // other types
                default:
                    std::cerr << "Error: Unsupported operator in binary operation\n";
                    exit(EXIT_FAILURE);
            }
        }

        void generateUnaryOp(const ast::UnaryOp *unaryOp, ir::IRCode &code) {
            generate(unaryOp->operand.get(), code);

            std::string dest = "t0";  // Simplification: assume result goes into "t0"
            std::string op1 = "t0";   // Operand should be in "t0" after generate()

            if (unaryOp->op == types::OperatorType::OP_MINUS) {
                code.emplace_back(ir::InstructionType::NEG, dest, op1);
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
            code.emplace_back(ir::InstructionType::LOAD_CONST, "t0", literal->value);
        }

        void generateIdentifier(const ast::Identifier *identifier, ir::IRCode &code) {
            code.emplace_back(ir::InstructionType::LOAD_VAR, "t0", identifier->name);
        }
    };
}

#endif