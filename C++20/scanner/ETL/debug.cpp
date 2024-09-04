#include<iostream>
#include<fstream>
#include<sstream>
#include"types.hpp"
#include"ir.hpp"

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

void outputDebugInfo(std::ostream &file, const ir::IRCode &code) {
    file << htmlPage_Header << "\n";
    file << "<tr><th>Index</th><th>Instruction</th>\n";
    uint64_t index = 0;
    for(const auto &i : code) {
        file << "<tr><td>" << index << "</td><td>" << i.toString() << "</td></tr>\n";
        index++;
    }
    //file << "</table>\n";
    //file <  "<table"
    file << htmlPage_Footer << "\n";
}
