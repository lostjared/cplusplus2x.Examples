#include "argz.hpp"
#include "cv_fractal.hpp"
#include <iostream>
#include <string>

class splitException {};

std::pair<double, double> splitPair(std::string_view src, std::string_view sep) {
	const auto pos{src.find(sep)};
	if(pos == std::string::npos)
		throw splitException{};
	std::string left, right;
	left = src.substr(0, pos);
	right = src.substr(pos + 1, src.length() - pos);
	if(left.length() == 0 || right.length() == 0)
		throw splitException{};
	const double val1{atof(left.c_str())}, val2{atof(right.c_str())};
	return std::make_pair(val1, val2);
}

int main(int argc, char **argv) {

	try {
		Argz<std::string> argz{argc, argv};
		argz.addOptionSingleValue('s', "image size WidthxHeight");
		argz.addOptionSingleValue('r', "real,imag values");
		argz.addOptionSingleValue('z', "zoom");
		argz.addOptionSingleValue('i', "iteration info");
		argz.addOptionSingleValue('c', "number of cores");
		argz.addOptionSingleValue('o', "output");
		argz.addOptionSingle('h', "help message");

		if(argc == 1) {
			argz.help(std::cout);
			return 0;
		}

		int value{};
		Argument<std::string> arg;
		std::string img_size;
		std::string real_size;
		std::string zoom_info;
		std::string iter_info;
		std::string core_info;
		std::string fname;

		while((value = argz.proc(arg)) != -1) {
			switch(value) {
			case 'h':
				argz.help(std::cout);
				return 0;
				break;
			case 's':
				img_size = arg.arg_value;
				break;
			case 'r':
				real_size = arg.arg_value;
				break;
			case 'z':
				zoom_info = arg.arg_value;
				break;
			case 'i':
				iter_info = arg.arg_value;
				break;
			case 'c':
				core_info = arg.arg_value;
				break;
			case 'o':
				fname = arg.arg_value;
				break;
			}
		}

		if(img_size.length() == 0 || real_size.length() == 0 || zoom_info.length() == 0 || iter_info.length() == 0 || core_info.length() == 0 || fname.length() == 0) {
			std::cerr << "Error missing argument..\n";
			argz.help(std::cout);
			return 0;
		}

		const std::pair<double, double> isize = splitPair(img_size, "x");
		const std::pair<double, double> m_values = splitPair(real_size, ",");
		const double zoom_value = atof(zoom_info.c_str());
		const int iter_value = atoi(iter_info.c_str());
		const int core_value = atoi(core_info.c_str());
		std::cout << "[ " << fname << " ]: Fractal -> " << static_cast<int>(isize.first) << "x" << static_cast<int>(isize.second) << " " << m_values.first << "," << m_values.second << " " << zoom_value << " " << iter_value << " " << core_value << "\n";
		//-0.743643887032151
		// 0.142625924205330
		cv::Mat m;
		m.create(cv::Size(static_cast<int>(isize.first), static_cast<int>(isize.second)), CV_8UC3);
		cv_fract::CV_Fractal cvfrac;
		cvfrac.initParameters(m_values.first, m_values.second, zoom_value, iter_value, core_value);
		cvfrac.resetPalette();
		cvfrac.draw(m);
		cv::imwrite(fname, m);
	} catch(const ArgException<std::string> &e) {
		std::cerr << "Syntax Error: " << e.text() << "\n";
		exit(0);
	} catch(const splitException &e) {
		std::cerr << "Exception has occoured...\n";
	} catch(...) {
		std::cerr << "Genereal exception has ocurred...\n";
	}
	return 0;
}
