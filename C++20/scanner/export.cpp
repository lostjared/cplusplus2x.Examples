#define VERSION_INFO "1.0"
#include <emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

extern int html_scanFile(const std::string &contents, std::ostream &file);

std::string test_parse(const std::string &data) {
    std::ostringstream stream;
    html_scanFile(data, stream);
    return stream.str();
}

class ETL_Export {
  public:
    ETL_Export() {
    }
    std::string scan(std::string input_line) {
        return test_parse(input_line);
    }
};

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_ETL) {
    class_<ETL_Export>("ETL")
        .constructor()
        .function("scan", &ETL_Export::scan);
}
