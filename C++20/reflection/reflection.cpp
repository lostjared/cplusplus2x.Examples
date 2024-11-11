#include<iostream>
#include<string>
#include<unordered_map>
#include<functional>

class Object {
public:
    void func1(std::string arg) {
        std::cout << "func1: " << arg << "\n";
        value = arg;
    }

    void func2(std::string arg) {
        std::cout << "func2: " << arg << "\n";
        value = arg;
    }

    void quit(std::string goodbye) {
        std::cout << "quit goodbye..\n";
        exit(EXIT_SUCCESS);
    }

    void printValue() {
        std::cout << "Value is: " << value << "\n";
    }
private:
    std::string value;
};

class Reflect {
public:
    std::unordered_map<std::string, std::function<void(Object &o, std::string)>> func;
    Reflect() {
        func["func1"] = [](Object &o, std::string arg) {
            o.func1(arg);
        };
        func["func2"] = [](Object &o, std::string arg) {
            o.func2(arg);
        };
        func["quit"] = [](Object &o, std::string arg) {
            o.quit(arg);
        };
    }

    void call(std::string f, Object &o, std::string arg) {
        auto f1 = func.find(f);
        if(f1 != func.end())
            f1->second(o, arg);
        else 
            std::cout << "Error bad function name..\n";
    }
};

int main() {
    Reflect reflection;
    Object obj1;
    while(1) {
        std::cout << "Enter func1, or func2 to specify function..\n";
        std::string line;
        std::getline(std::cin, line);
        reflection.call(line, obj1, line);
        obj1.printValue();
    }
    return 0;
}