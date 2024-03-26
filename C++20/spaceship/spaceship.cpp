#include <iostream>

class Rect {
   public:
	int x, y, w, h;
	auto operator<=>(const Rect &r) const = default;
};

class Point {
   public:
	int x, y;
	// only order by x
	auto operator<=>(const Point &p) const { return (x <=> p.x); }
};

int main(int argc, char **argv) {
	Rect r{0, 0, 640, 480}, r2{10, 10, 1024, 720};
	Rect r3{0, 0, 640, 480};
	if (r == r2) {
		std::cout << "Equal\n";
	} else {
		std::cout << "Not Equal\n";
	}
	if (r == r3) {
		std::cout << "r3 is equal\n";
	}
	if (r < r2) {
		std::cout << "less than..\n";
	}
	if (r != r2 && r >= r3) {
		std::cout << "True!\n";
	} else {
		std::cout << "false..\n";
	}
	Point p1{0, 100}, p2{150, 150};

	if (p1 < p2) {
		std::cout << "less than\n";
	} else {
		std::cout << "not less than\n";
	}
	p1.x = 250;
	if (p1 < p2) {
		std::cout << "less than again\n";
	} else {
		std::cout << "Only order by x.\n";
	}
	return 0;
}
