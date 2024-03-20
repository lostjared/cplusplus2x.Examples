#include<iostream>
#include<type_traits>
#include<vector>

template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename C, Numeric T1, Numeric T2>
void push_add(C &cont, T1 val1, T2 val2) {
    cont.push_back(val1+val2);
}

template<typename C, typename T1, typename T2>
requires (std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
void push_sub(C &cont, T1 type1, T2 type2) {
    cont.push_back(type1-type2);
}

int main() {
    std::vector v{0, 1, 2};
    push_add(v, 1, 1.5); // okay narrow 1.5
    //push_add(v, 1, "Two"); // ERR requires arithmetic type
    
    push_sub(v, 3, 2);
    push_sub(v, 3, 3.5f); // narrow
    //push_sub(v, "test", "test"); // ERR requires arithmetic
    
    for(auto &i : v) {
        std::cout << "index: " << i << "\n";
    }
    return 0;
}
