#include<iostream>
#include<syncstream>
#include<cstdlib>
#include<thread>

void output_string(const std::string &o) {
    std::osyncstream stream(std::cout);
    stream << o << "\n";
}

int main() {
 
    std::cout << "Sync: {\n";
    std::thread t1(output_string, "Hello World #1");
    std::thread t2(output_string, "Hello World #2");
    t1.join();
    t2.join();
    std::cout << "}\n";
    return EXIT_SUCCESS;
}