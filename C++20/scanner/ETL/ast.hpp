#include<memory>
#include<string>
#include<vector>

namespace ast {

    struct ASTNode {
        virtual ~ASTNode() = default;
        virtual std::string text() const = 0;  // Pure virtual method to be implemented by subclasses
    };

    struct Expression : ASTNode {
    };

    struct Statement : ASTNode {
        ~Statement() override = default;
    };

    struct Literal : Expression {
        std::string value;

        Literal(const std::string &v) : value(v) {}

        std::string text() const override {
            return value;  
        }
    };

    struct Identifier : Expression {
        std::string name;

        Identifier(const std::string &n) : name(n) {}

        std::string text() const override {
            return name;  
        }
    };

    struct BinaryOp : Expression {
        types::OperatorType op;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        BinaryOp(types::OperatorType o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
            : op(o), left(std::move(l)), right(std::move(r)) {}

        std::string text() const override {
            return "(" + left->text() + " " + types::opName[static_cast<unsigned int>(op)] + " " + right->text() + ")";
        }
    };

    struct UnaryOp : Expression {
        types::OperatorType op;
        std::unique_ptr<Expression> operand;

        UnaryOp(types::OperatorType o, std::unique_ptr<Expression> e)
            : op(o), operand(std::move(e)) {}

        std::string text() const override {
            return types::opName[static_cast<unsigned int>(op)] + operand->text();
        }
    };

    struct Call : Statement, Expression {
        std::string functionName;
        std::vector<std::unique_ptr<Expression>> arguments;

        Call(const std::string &name, std::vector<std::unique_ptr<Expression>> args)
            : functionName(name), arguments(std::move(args)) {}

        std::string text() const override {
            std::string result = functionName + "(";
            for (size_t i = 0; i < arguments.size(); ++i) {
                result += arguments[i]->text();
                if (i < arguments.size() - 1) {
                    result += ", ";
                }
            }
            result += ")";
            return result;
        }
    };

    struct Assignment : Statement {
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        Assignment(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
            : left(std::move(l)), right(std::move(r)) {}

        std::string text() const override {
            return left->text() + " = " + right->text() + ";";
        }
    };

    struct Return : Statement {
        std::unique_ptr<Expression> return_value;
        Return(std::unique_ptr<Expression> rt) : return_value(std::move(rt)) {}
        std::string text() const override {
            return "return " + return_value->text() + ";";
        }
    };

    struct Function : ASTNode {
        std::string name;
        std::vector<std::unique_ptr<ASTNode>> body;

        Function(const std::string &n) : name(n) {}

        std::string text() const override {
            std::string result = "function " + name + "() {\n";
            for (const auto& stmt : body) {
                result += "  " + stmt->text() + "\n";
            }
            result += "}";
            return result;
        }
    };

    struct Program : ASTNode {
        std::vector<std::unique_ptr<ASTNode>> body;

        Program() = default;

        std::string text() const override {
            std::string result;
            for (const auto& stmt : body) {
                result += stmt->text() + "\n";
            }
            return result;
        }
    };

}