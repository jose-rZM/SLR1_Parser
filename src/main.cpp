#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../include/slr1_parser.hpp"

int main(int argc, char** argv) {
    if (argc != 2)
        return (1);

    std::string grammar_file = argv[1];
    SLR1Parser  slrparser{grammar_file};
    slrparser.make_parser();
    slrparser.debugStates();
    return (0);
}
