
// page 194

#include <iostream>
#include <string>

template <size_t N>
struct Data {
	char data[N];
	const char *c_str() { return data; }
};

template <size_t N>
Data(const char (&)[N]) -> Data<N>;

template <auto prefix>
class Log {
  public:
	void log(std::string_view s) const { std::cout << prefix.data << " -> " << s << "\n"; }
};

int main(int argc, char **argv) {
	Log<Data{"Apples: "}> logger;
	logger.log("By the skin of my teeth");
	return 0;
}