#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdint>

template<typename T>
class Color {
public:
    T rgb[3];
    auto operator<=>(const Color<T> &c) const {
        return (color() <=> c.color());
    }
    size_t color() const { return rgb[0]+rgb[1]+rgb[2]; }
    friend std::ostream &operator<<(std::ostream &out, const Color<T> &c) {
        out << static_cast<size_t>(c.rgb[0]) << "," << static_cast<size_t>(c.rgb[1]) << "," << static_cast<size_t>(c.rgb[2]) << " -> " << static_cast<size_t>(c.color());
        return out;
    }
};


int main(int argc, char **argv) {
    std::vector<Color<uint8_t>> colors {{255,255,255}, {150, 50, 50}, {250, 100, 25}};

    std::cout << "8 bit: ->\n";
    
    std::sort(colors.begin(), colors.end(), [](const Color<uint8_t> &c1, const Color<uint8_t> &c2) {
        return (c2 < c1);
    });
    std::cout << "Pixel Sorted:\n";
    for(const auto &i : colors) {
        std::cout << i << "\n";
    }
    
    std::cout << "16 bit (reverse order) ->\n";
    std::vector<Color<uint16_t>> color_sixteen { {1024, 500, 25 }, {2048, 128,55}, {1000,2000,32}};
    std::sort(color_sixteen.begin(), color_sixteen.end());
    
    for(const auto &j : color_sixteen) {
        std::cout << j << "\n";
    }
    
    
    
    return 0;
}

