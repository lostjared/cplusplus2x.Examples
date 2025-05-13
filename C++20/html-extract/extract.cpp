#include <iostream>
#include <fstream>
#include <sstream>
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " input.html\n";
        return 1;
    }
    std::string inputPath = argv[1];
    auto dotPos = inputPath.find_last_of('.');
    std::string base = (dotPos == std::string::npos) ? inputPath : inputPath.substr(0, dotPos);
    std::string cssPath = base + ".css";
    std::string jsPath = base + ".js";
    std::string outputHtmlPath = base + ".separated.html";
    std::ifstream inFile(inputPath);
    if (!inFile) {
        std::cerr << "Error: unable to open " << inputPath << "\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    std::string allCss;
    std::string allJs;

    size_t pos = 0;
    while (true) {
        auto start = content.find("<style", pos);
        if (start == std::string::npos) break;
        auto startTagEnd = content.find('>', start);
        if (startTagEnd == std::string::npos) break;
        auto end = content.find("</style>", startTagEnd);
        if (end == std::string::npos) break;
        allCss += content.substr(startTagEnd + 1, end - (startTagEnd + 1)) + "\n";
        std::string linkTag = "<link rel=\"stylesheet\" href=\"" + cssPath + "\">";
        content.replace(start, (end + 8) - start, linkTag);
        pos = start + linkTag.size();
    }

    pos = 0;
    while (true) {
        auto start = content.find("<script", pos);
        if (start == std::string::npos) break;
        auto startTagEnd = content.find('>', start);
        if (startTagEnd == std::string::npos) break;
        auto end = content.find("</script>", startTagEnd);
        if (end == std::string::npos) break;
        allJs += content.substr(startTagEnd + 1, end - (startTagEnd + 1)) + "\n";
        std::string scriptTag = "<script src=\"" + jsPath + "\"></script>";
        content.replace(start, (end + 9) - start, scriptTag);
        pos = start + scriptTag.size();
    }

    std::ofstream cssOut(cssPath);
    cssOut << allCss;
    std::ofstream jsOut(jsPath);
    jsOut << allJs;
    std::ofstream htmlOut(outputHtmlPath);
    htmlOut << content;

    std::cout << "Separated files generated:\n"
              << "  HTML: " << outputHtmlPath << "\n"
              << "  CSS:  " << cssPath      << "\n"
              << "  JS:   " << jsPath       << "\n";
    return 0;
}
