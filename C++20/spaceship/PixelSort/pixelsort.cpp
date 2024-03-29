#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
class Color {
public:
	T rgb[3];
	auto operator<=>(const Color<T> &c) const { return (color() <=> c.color()); }
	const size_t color() const { return rgb[0] + rgb[1] + rgb[2]; }
	friend std::ostream &operator<<(std::ostream &out, const Color<T> &c) {
		out << c.hex() << ": " << std::setw(10) << static_cast<size_t>(c.rgb[0]) << "," << std::setw(10) << static_cast<size_t>(c.rgb[1]) << "," << std::setw(10) << static_cast<size_t>(c.rgb[2]) << " -> " << std::setw(10) << c.color();
		return out;
	}

	const std::string hex() const {
		std::ostringstream stream;
		stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[0]) << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[1]) << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[2]);
		return stream.str();
	}
};

template <typename T, typename V>
bool pixelSort(const cv::Mat &frame, cv::Mat &output, bool direction) {

	if(frame.empty())
		return false;

	if(frame.channels() < 3)
		return false;

	output = frame.clone();
	for(int z = 0; z < frame.rows; ++z) {
		std::vector<Color<T>> line;
		for(int i = 0; i < frame.cols; ++i) {
			V pixel{frame.at<V>(z, i)};
			line.push_back({pixel[2], pixel[1], pixel[0]}); // bgr -> rgb
		}
		std::sort(line.begin(), line.end(), [=](const Color<T> &c1, const Color<T> &c2) {
			if(direction == true)
				return (c2 < c1);
			return (c1 < c2);
		});
		for(int i = 0; i < frame.cols; ++i) {
			V pix{V(line[i].rgb[2], line[i].rgb[1], line[i].rgb[0])}; // rgb -> bgr
			output.at<V>(z, i) = pix;
		}
	}
	return true;
}

int main(int argc, char **argv) {
	if(argc != 4) {
		std::cerr << argv[0] << " input.png output.png (1|2)\n1 for Normal, 2 for Reverse\n";
		return 0;
	}

	bool dir{false};
	if(argv[3] == std::string("1"))
		dir = true;

	const cv::Mat image{cv::imread(argv[1])};
	if(image.empty()) {
		std::cerr << "Could not load image..\n";
		return 0;
	}
	cv::Mat output;
	if(pixelSort<uint8_t, cv::Vec3b>(image, output, dir)) {
		cv::imwrite(argv[2], output);
		std::cout << "wrote: " << argv[2] << "\n";
	} else {
		std::cerr << "Error occurred..\n";
	}
	return 0;
}
