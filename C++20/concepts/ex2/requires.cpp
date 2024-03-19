#include<iostream>
#include<type_traits>


template<typename T>
concept is_signed_int = std::is_signed_v<T> &&  !std::is_floating_point_v<T>;

template<typename T>
requires(std::is_signed_v<T> && !std::is_floating_point_v<T>)
void count_back(T start) {
    for(T i = start; i > 0; i--) {
        std::cout << "index: " << i << "\n";
    }
}

template<is_signed_int I>
void count_back(I start, I skip) {
    for(I i = start; i > 0; i -= skip) {
        std::cout << "index: " << i << "\n";
    }
}

int main(int argc, char **argv) {
    count_back(10);
    count_back(20, 2);
 //   count_back(static_cast<unsigned int>(10)); ERR
    return 0;
}
