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

    std::string                 grammar_file = argv[1];
    SLR1Parser                  slrparser{grammar_file};
    std::unordered_set<Lr0Item> items;
    items.insert({"A", {"function", "ap", "A", "cp", "B"}, 4});
    ;
    slrparser.closure(items);
    std::cout << "ITEMS LR0\n";
    for (const auto& item : items) {
        item.printItem();
        std::cout << "\n";
    }
    std::cout << std::endl;

    return (0);
}
