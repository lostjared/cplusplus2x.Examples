#include "../pattern.hpp"
#include<filesystem>

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
                cv::resize(seed, seed, cv::Size(w, h));
                cv::namedWindow("frame", cv::WINDOW_AUTOSIZE);
                std::cout << "Frame size: " << static_cast<int>(w) << "x" << static_cast<int>(h) << "\n";

                bool active = true;
                while (active) {
                    cv::Mat frame;
                    cap >> frame;
                    if (frame.empty()) {
                        active = false;
                        break;
                    }
                    map_pattern(frame, pattern, seed);
                    pattern.shift(1);
                    cv::imshow("frame", frame);
                    if (cv::waitKey(1) == 27) {
                        active = false;
                        break;
                    }
                }
                cap.release();
            }

        } catch (std::runtime_error &err) {
            std::cerr << err.what() << "\n";
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
