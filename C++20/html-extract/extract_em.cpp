#include<iostream>
#include<string>

#include <emscripten/bind.h>

class Extractor {
public:

    std::string getCss() { return css; }
    std::string getJs() { return js; }
    std::string getHtml() { return html; }

    bool extractHtml(const std::string& content) {
        std::string modifiedContent = content; // Make a copy to modify
        std::string allCss;
        std::string allJs;

        size_t pos = 0;
        while (true) {
            auto start = modifiedContent.find("<style", pos);
            if (start == std::string::npos) break;
            auto startTagEnd = modifiedContent.find('>', start);
            if (startTagEnd == std::string::npos) break;
            auto end = modifiedContent.find("</style>", startTagEnd);
            if (end == std::string::npos) break;
            allCss += modifiedContent.substr(startTagEnd + 1, end - (startTagEnd + 1)) + "\n";
            std::string linkTag = "<link rel=\"stylesheet\" href=\"" + cssPath + "\">";
            modifiedContent.replace(start, (end + 8) - start, linkTag);
            pos = start + linkTag.size();
        }

        pos = 0;
        while (true) {
            auto start = modifiedContent.find("<script", pos);
            if (start == std::string::npos) break;
            auto startTagEnd = modifiedContent.find('>', start);
            if (startTagEnd == std::string::npos) break;
            auto end = modifiedContent.find("</script>", startTagEnd);
            if (end == std::string::npos) break;
            allJs += modifiedContent.substr(startTagEnd + 1, end - (startTagEnd + 1)) + "\n";
            std::string scriptTag = "<script src=\"" + jsPath + "\"></script>";
            modifiedContent.replace(start, (end + 9) - start, scriptTag);
            pos = start + scriptTag.size();
        }

        this->css = allCss;
        this->js = allJs;
        this->html = modifiedContent;

        return true;
    }
private:
    std::string css;
    std::string js;
    std::string html;
    std::string cssPath = "styles.css";
    std::string jsPath = "script.js";
};

EMSCRIPTEN_BINDINGS(extractor_module) {
    emscripten::class_<Extractor>("Extractor")
        .constructor<>()
        .function("getCss", &Extractor::getCss)
        .function("getJs", &Extractor::getJs)
        .function("getHtml", &Extractor::getHtml)
        .function("extractHtml", &Extractor::extractHtml);
}