#include<iostream>

class Rect {
public:
	int x, y, w, h;
	auto operator <=> (const Rect &r) const = default;
};

int main(int argc, char **argv) {
	Rect r {0, 0, 640, 480}, r2 {10, 10, 1024, 720};
	Rect r3 {0,0, 640, 480};
	if(r == r2) {
		std::cout << "Equal\n";
	} else {
		std::cout << "Not Equal\n";
	}
	if(r == r3) {
		std::cout << "r3 is equal\n";
	}
	if(r < r2) {
		std::cout << "less than..\n";
	}
	return 0;
}
