#include<iostream>
#include<vector>

template<typename T>
class Color {
public:
    T red,green,blue;
    auto operator<=>(const Color<T> &c) const {
        return (color() <=> c.color());
    }
    size_t color() const { return red+green+blue; }
    friend std::ostream &operator<<(std::ostream &out, const Color<T> &c) {
        out << static_cast<size_t>(c.red) << "," << static_cast<size_t>(c.green) << "," << static_cast<size_t>(c.blue) << " -> " << static_cast<size_t>(c.color());
        return out;
    }
};


int main(int argc, char **argv) {
    std::vector<Color<unsigned char>> colors;
    colors.push_back({255,255,255});
    colors.push_back({150, 50, 50});
    colors.push_back({250, 100, 25});
    
    std::sort(colors.begin(), colors.end(), [](const Color<unsigned char> &c1, const Color<unsigned char> &c2) {
        return (c2.color() < c1.color());
    });
    
    std::cout << "Pixel Sorted:\n";
    for(const auto &i : colors) {
        std::cout << i << "\n";
    }
    
    return 0;
}

