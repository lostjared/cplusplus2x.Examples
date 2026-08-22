#include "../pattern.hpp"
#include <filesystem>

int main(int argc, char **argv) {
    if (argc == 4) {
        Pattern<std::string> pattern;
        try {
            std::string file = argv[1];
            if (std::filesystem::exists(file)) {
                read_file<std::string>(argv[2], pattern);
                cv::VideoCapture cap;
                cap.open(file);
                if (!cap.isOpened()) {
                    throw std::runtime_error("Error opening camera");
                }
                cv::Mat seed;
                cv::imread(argv[3], seed);
                if (seed.empty())
                    throw std::runtime_error("Error opening image: " + std::string(argv[3]));

                double w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
                double h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
                double fps = cap.get(cv::CAP_PROP_FPS);
                cv::resize(seed, seed, cv::Size(w, h));
                cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
                cv::resizeWindow("frame", static_cast<int>(w), static_cast<int>(h));
                std::cout << "Frame size: " << static_cast<int>(w) << "x" << static_cast<int>(h) << "x" << fps << "\n";
                cv::VideoWriter writer;
                writer.open("output.mp4", CV_FOURCC('m', 'p', '4', 'v'), fps, cv::Size(w, h));

                bool active = true;
                while (active) {
                    cv::Mat frame;
                    cap >> frame;
                    if (frame.empty()) {
                        active = false;
                        break;
                    }
                    map_pattern(frame, pattern, seed);
                    writer.write(frame);
                    pattern.shift(1);
                    cv::imshow("frame", frame);
                    int key = 0;
                    switch (cv::waitKey(1)) {
                    case 27:
                        active = false;
                        break;
                    case 'z':
                    case 'Z': {
                        static int file_index = 0;
                        cv::imwrite(std::string("frame_") + std::to_string(file_index++) + ".png", frame);
                    } break;
                    }
                }
                cap.release();
                writer.release();
            }

        } catch (std::runtime_error &err) {
            std::cerr << err.what() << "\n";
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
