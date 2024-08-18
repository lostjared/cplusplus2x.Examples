#include"scanner.hpp"
#include<fstream>
#include<sstream>
#include<iostream>
#include<memory>

std::string htmlPage_Header = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Token Table</title>
    <style>
        table {
            width: 50%;
            margin: 50px auto;
            border-collapse: collapse;
        }
        th, td {
            border: 1px solid #000;
            padding: 10px;
            text-align: center;
            background-color: #f2f2f2;
        }
        th {
            background-color: #000000;
            color: #f2f2f2;
        }
    </style>
</head>
<body>
<table>
)";

std::string htmlPage_Footer = R"(
</table>
</body>
</html>
)";

int html_scanFile(const std::string &contents, const char *out_filename) {
    try {


        std::fstream file;
        file.open(out_filename, std::ios::out);
        if(!file.is_open()) {
            std::cerr << "Error could not open output file: " << out_filename << "\n";
            exit(EXIT_FAILURE);
        }
        file << htmlPage_Header << "\n";

        std::unique_ptr<scan::Scanner> scan(new scan::Scanner(scan::TString(contents)));
        uint64_t tokens = scan->scan();
        std::cout << "scan: Sucessfull scan returned: " << tokens << " token(s)...\n";

        file << "<tr><th>Index/Line:Col</th><th>Token</th><th>Type</th></tr>";

        for(size_t i = 0; i < scan->size(); ++i) {
            auto posx = scan->operator[](i).get_pos();
            file << "<tr><td>" << i << "/" << posx.first << ":" << posx.second << "</td><td>" << scan->operator[](i).getTokenValue() << "</td><td>";
            types::print_type_TokenType(file, scan->operator[](i).getTokenType());
            file << "</td></tr>";
        }
        file.close();
        file << "scanner: Wrote "<< out_filename << "\n";
        return static_cast<int>(tokens);

    } catch(scan::ScanExcept &e) {
        std::cerr << "Fatal error: " << e.why() << "\n";
    }
    return 0;
}

int html_main(const char *filename, const char *outfilename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();

    if(stream.str().length()>0) {
        return html_scanFile(stream.str(), outfilename);
    }
    return 0;
}
