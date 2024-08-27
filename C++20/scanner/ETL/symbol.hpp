#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <unordered_map>
#include <optional>
#include <string>

namespace symbol {


     struct Symbol {
        std::string name;
        std::string value;
        int ivalue;
        double dvalue;
        Symbol() : name{}, value{}, ivalue{}, dvalue{} {}
        Symbol(const Symbol &s) : name{s.name}, value{s.value}, ivalue{s.ivalue}, dvalue{s.dvalue} {}
        Symbol &operator=(const Symbol &s) {
            name = s.name;
            value = s.value;
            ivalue = s.ivalue;
            dvalue = s.dvalue;
            return *this;
        }
    };

    class SymbolTable {
    public:
        SymbolTable() {
            symbols["global"] = std::unordered_map<std::string, Symbol>();
            cur_scope = &symbols["global"];  // Initialize cur_scope to point to the global scope
        }

        void enter(const std::string &sym) {
            (*cur_scope)[sym] = Symbol();
        }

        void enterScope(const std::string &fname) {
            if (symbols.find(fname) == symbols.end()) {
                symbols[fname] = std::unordered_map<std::string, Symbol>();
            }
            cur_scope = &symbols[fname];
        }

        void exitScope() {
            cur_scope = &symbols["global"];
        }

        std::optional<Symbol*> lookup(const std::string &sym) {
            auto it = cur_scope->find(sym);
            if (it != cur_scope->end())
                return &it->second;
            return std::nullopt;
        }
        
        int getCurrentScopeSize() const {
            return cur_scope->size();
        }
        
        auto getSymbols() {
            return symbols;
        }

        // New method to check if a symbol is in memory
        bool isInMemory(const std::string &sym) const {
            return cur_scope->find(sym) != cur_scope->end();
        }

        void print() {
            for(auto &i : symbols) {
                for(auto &z : i.second) {
                    std::cout << z.second.name << " : " << z.second.value << "\n";
                }
            }
        }

    private:
        std::unordered_map<std::string, std::unordered_map<std::string, Symbol>> symbols;
        std::unordered_map<std::string, Symbol> *cur_scope;  // Pointer to the current scope
    };

}

#endif