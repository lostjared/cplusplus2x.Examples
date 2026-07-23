#include <Eigen/Dense>
#include <iostream>

int main() {
    Eigen::Matrix2d left;
    Eigen::Matrix2d right;
    left << 1.0, 2.0, 3.0, 4.0;
    right << 5.0, 6.0, 7.0, 8.0;
    const Eigen::Matrix2d product = left * right;
    std::cout << product << '\n';
    return 0;
}
