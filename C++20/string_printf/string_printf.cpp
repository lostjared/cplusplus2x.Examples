#include <iostream>
#include <sstream>
#include <string>

namespace io {

	std::ostringstream stream;

	std::string string_printf(const char *s) {
		if (s != nullptr) {
			stream << s;
		}
		std::string rtval = stream.str();
		stream.str("");
		stream.clear();
		return rtval;
	}

	template <typename T, typename... Args>
	std::string string_printf(const char *s, T value, Args... args) {
		while (s && *s) {
			if (*s == '%' && *++s != '%') {
				stream << value;
				return string_printf(++s, args...);
			}
			stream << *s++;
		}
		if (sizeof...(args) > 0)
			throw std::runtime_error("Extra arguments...\n");
		std::string rt = stream.str();
		stream.str("");
		stream.clear();
		return rt;
	}
} // namespace io

int main() {
	std::string s = io::string_printf("test %s %d %f", "two", 10, 25.3f);
	std::cout << s << "\n";
	return 0;
}