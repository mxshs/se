#include "utils.h"

std::string formatIndent(std::vector<std::string> rows, int depth) {
    std::string out = "";
    std::string indent = "";
    indent.resize(depth, '\t');

    for (std::string row: rows) {
        out += indent + row + "\n";
    }

    return out;
}

