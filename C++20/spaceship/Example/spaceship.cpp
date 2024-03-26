#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T> class Color {
   public:
	T rgb[3];

	auto operator<=>(const Color<T> &c) const {
		return (color() <=> c.color());
	}

	const size_t color() const { return rgb[0] + rgb[1] + rgb[2]; }

	friend std::ostream &operator<<(std::ostream &out, const Color<T> &c) {
		out << c.hex() << ": " << std::setw(10)
		    << static_cast<size_t>(c.rgb[0]) << "," << std::setw(10)
		    << static_cast<size_t>(c.rgb[1]) << "," << std::setw(10)
		    << static_cast<size_t>(c.rgb[2]) << " -> " << std::setw(10)
		    << c.color();
		return out;
	}

	const std::string hex() const {
		std::ostringstream stream;
		stream << std::hex << std::uppercase << std::setfill('0')
			  << std::setw(2) << static_cast<size_t>(rgb[0]) << std::hex
			  << std::uppercase << std::setfill('0') << std::setw(2)
			  << static_cast<size_t>(rgb[1]) << std::hex << std::uppercase
			  << std::setfill('0') << std::setw(2)
			  << static_cast<size_t>(rgb[2]);
		return stream.str();
	}
};

int main(int argc, char **argv) {
	std::vector<Color<uint8_t>> colors{
	    {255, 255, 255}, {150, 50, 50}, {250, 100, 25}};
	std::cout << "8 bit: ->\n";
	std::sort(colors.begin(), colors.end(),
			[](const Color<uint8_t> &c1, const Color<uint8_t> &c2) {
				return (c2 < c1);
			});
	std::cout << "Pixel Sorted:\n";
	std::cout << std::setw(7) << "Hex" << std::setw(11) << "R" << std::setw(11)
			<< "G" << std::setw(11) << "B" << "  " << std::setw(13)
			<< "Total\n";

	for (const auto &i : colors) {
		std::cout << i << "\n";
	}
	std::sort(colors.begin(), colors.end());
	std::cout << "Pixel Sorted (Reverse):\n";
	for (const auto &i : colors) {
		std::cout << i << "\n";
	}
	return 0;
}
