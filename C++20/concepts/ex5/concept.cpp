#include<iostream>
#include<type_traits>
#include<vector>

template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// add and return as same type as container C
template<typename C, Numeric T1, Numeric T2>
auto push_add(C &cont, T1 val1, T2 val2) -> typename C::value_type {
    cont.push_back(val1+val2);
    return val1+val2;
}
// sub and return type deduced by type1-type2
template<typename C, typename T1, typename T2>
requires (std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
auto push_sub(C &cont, T1 type1, T2 type2) {
    cont.push_back(type1-type2);
    return type1-type2;
}

int main() {
    std::vector v{0, 1, 2};
    auto num = push_add(v, 1, 1.5); // okay narrow 1.5
    std::cout << "Narrow add on v<int>: " << num << "\n";
    //push_add(v, 1, "Two"); // ERR requires arithmetic type
    push_sub(v, 3, 2);
    auto rt = push_sub(v, 3, 3.5f); // narrow / return double without narrow
    std::cout << "sub on v<int> return: " << rt << "\n";
    //push_sub(v, "test", "test"); // ERR requires arithmetic
    std::vector v2 { 0.5, 0.1, 2.0 };
    auto rt_v = push_add(v2, 0.5, 1);
    std::cout << "add on v2<double> return type: " << rt_v << "\n";
    for(auto &i : v) {
        std::cout << "index: " << i << "\n";
    }
    return 0;
}
