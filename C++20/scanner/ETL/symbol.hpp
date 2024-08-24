#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include<unordered_map>
#include<optional>

namespace symbol {

    struct Symbol {
        std::string name;
        std::string value;
        int ivalue;
        double dvalue;
        Symbol() : name{}, value{}, ivalue{}, dvalue{} {}
    };

    class SymbolTable  {
    public:
        SymbolTable() { }
        void enter(const std::string &sym) {
            symbols[sym] = Symbol();
        }
        std::optional<Symbol> lookup(const std::string &sym) {
            auto it = symbols.find(sym);
            if(it != symbols.end())
                return it->second;
            return std::nullopt;
        }

        std::unordered_map<std::string, Symbol> &getSymbols() { return symbols; }

    private:
        std::unordered_map<std::string, Symbol> symbols;
    };

}



#endif