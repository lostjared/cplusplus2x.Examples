#include<iostream>
#include<string>



class X {
public:
    std::string name;
    void func() {
        int index = 0;
        auto it = [&, this]() {
            index = 1;
            name = "test";
        };
        it();
        std::cout << name << " index: " << index << "\n";
        auto it_value = [=, *this]() {
            std::cout << "index +1 =" << (index+1) << "\n";
        };
        it_value();
    }
    auto test2() {
        auto it =[&, this]() {
            name = " abc 1 2 3";
        };
        return it;
    }
};

int main() {
    X x;
    x.func();
    auto it = x.test2();
    it();
    std::cout << x.name << "\n";
    return 0;
}