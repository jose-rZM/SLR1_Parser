#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../include/slr1_parser.hpp"

int main(int argc, char** argv) {
    //if (argc != 2) return (1);

    std::string grammar_file = "owo.txt";
    SLR1Parser slrparser{grammar_file};
    if (!slrparser.make_parser()) {
        std::cout << "Conflict\n";
        return 1;
    }
    slrparser.debugStates();
    slrparser.debugTable();
    slrparser.debugActions();
    std::vector<std::string> input{"IDENT", "mult", "IDENT"};
    if (slrparser.parse(input)) {
        std::cout << "Parsing was successful" << std::endl;
    } else {
        std::cout << "Error while parsing" << std::endl;
    }
    return (0);
}
