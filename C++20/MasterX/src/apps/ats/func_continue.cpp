#include "code.hpp"
#include "function.hpp"

namespace token {

    void token_Continue(const std::string &cmd, std::vector<lex::Token> &tokens) {

        if (code.instruct.size() == 0) {
            stream << "Error: requires you run build first.\r\n";
            return;
        }

        code.cont();
    }

} // namespace token
