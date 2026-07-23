#include <Eigen/Dense>
#include <iostream>

int main() {
    Eigen::Matrix2d left;
    Eigen::Matrix2d right;
    left << 1.0, 2.0, 3.0, 4.0;
    right << 5.0, 6.0, 7.0, 8.0;
    std::cout << "Left:\n" << left << "\n";
    std::cout << "Right:\n" << right << "\n";
    Eigen::Matrix2d one, two;
    one << 5.0, 4.0, 3.0, 2.0;
    two << 8.0, 7.0, 6.0, 5.0;
    std::cout << "one:\n" << one << "\n";
    std::cout << "two:\n" << two << "\n";
    Eigen::Matrix2d product = left * right * one  * two;
    std::cout << "Product:\n" <<  product << '\n';
    return 0;
}
