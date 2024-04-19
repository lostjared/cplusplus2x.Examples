#define _m(Obj)  int main(int argc, char **argv) { return Obj::main(argc, argv); }
#include<iostream>
#include<string>

class Object {
    public: Object() { std::cout << "Hey \n"; }

    public: void test() {
        std::cout << "Test 1 2 3...\n";
    }

    public: static int main(int argc, char **argv) {
        std::cout << "Hello, World\n";
        Object o;
        o.test();
        return 0;
    }
};

_m(Object)