#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdint>
#include<string>
#include<sstream>
#include<iomanip>


template<typename T>
class Color {
public:
    T rgb[3];
    auto operator<=>(const Color<T> &c) const {
        return (color() <=> c.color());
    }
    const size_t color() const { return rgb[0]+rgb[1]+rgb[2]; }
    friend std::ostream &operator<<(std::ostream &out, const Color<T> &c) {
        out << c.hex() << ": " << std::setw(10) << static_cast<size_t>(c.rgb[0]) << "," << std::setw(10) << static_cast<size_t>(c.rgb[1]) << "," << std::setw(10) << static_cast<size_t>(c.rgb[2]) << " -> " << std::setw(10) << static_cast<size_t>(c.color());
        return out;
    }
    
    const std::string hex() const {
        std::ostringstream stream;
        stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[0]) << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[1])<< std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<size_t>(rgb[2]);
        return stream.str();
    }
};

void pixelSort(const cv::Mat &frame, cv::Mat &output) {
    output = frame.clone();
    for(int z = 0; z < frame.rows; ++z) {
        std::vector<Color<uint8_t>> line;
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b pixel {frame.at<cv::Vec3b>(z, i)};
            line.push_back({pixel[2], pixel[1], pixel[0]}); // bgr -> rgb
        }
        std::sort(line.begin(), line.end());
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b pix {cv::Vec3b(line[i].rgb[2], line[i].rgb[1], line[i].rgb[0])}; // rgb -> bgr
            output.at<cv::Vec3b>(z, i) = pix;
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 3) {
        std::cerr << argv[0] << " input.png output.png\n";
        return 0;
    }
    cv::Mat image{cv::imread(argv[1])};
    if(image.empty()) {
        std::cerr << "Could not load image..\n";
    }
    cv::Mat output;
    pixelSort(image, output);
    cv::imwrite(argv[2], output);
    std::cerr << "wrote: " << argv[2] << "\n";
    return 0;
}
