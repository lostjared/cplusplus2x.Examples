#include<opencv2/opencv.hpp>
#include"mdspan.hpp" // in the ftuure when package
// as a part of libstdc++ change to
// #include<mdspan>
#include<print> 

int main() {
    int width = 800;
    int height = 600;
    cv::Mat img(height, width, CV_8UC3); // 3 Channel BGR
    if (!img.isContinuous()) {
        std::println("Error: Mat is not continuous! mdspan default layout won't work.");
        return -1;
    }
    auto grid = std::mdspan(img.ptr<cv::Vec3b>(), height, width);
    std::println("Drawing gradient on {}x{} image...", width, height);
    for (size_t y = 0; y < grid.extent(0); ++y) {
        for (size_t x = 0; x < grid.extent(1); ++x) {
            uint8_t blue = (x * 255) / width;
            uint8_t green = (y * 255) / height;
            uint8_t red = 0;
            grid[y, x] = cv::Vec3b(blue, green, red); 
        }
    }
    cv::imwrite("gradient.png", img);
    std::println("Saved 'gradient.png'.");
    return 0;
}
