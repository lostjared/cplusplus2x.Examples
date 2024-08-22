#include<memory>
#include<string>
#include<vector>


namespace ast {
    struct ASTNode {
        virtual ~ASTNode() = default;
    };

    struct Expression : ASTNode {
        // Members for expression representation
    };

    struct Assignment : ASTNode {
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
        Assignment(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
            : left(std::move(l)), right(std::move(r)) {}
    };

    struct Function : ASTNode {
        std::string name;
        std::vector<std::unique_ptr<ASTNode>> body;
        Function(const std::string &n) : name(n) {}
    };

    struct Identifier : Expression {
        std::string name;
        Identifier(const std::string &n) : name(n) {}
    };

    struct BinaryOp : Expression {
        types::OperatorType op;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        BinaryOp(types::OperatorType o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
            : op(o), left(std::move(l)), right(std::move(r)) {}
    };

    struct Literal : Expression {
        std::string value;
        Literal(const std::string &v) : value(v) {}
    };

    struct Program : ASTNode {
        std::vector<std::unique_ptr<ASTNode>> body;
    };

}