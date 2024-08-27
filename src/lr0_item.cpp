#include <iostream> // DEBUG ONLY
#include <string>
#include <utility>
#include <vector>

#include "../include/lr0_item.hpp"
#include "../include/symbol_table.hpp"

Lr0Item::Lr0Item(std::string antecedent, std::vector<std::string> consequent)
    : antecedent(std::move(antecedent)), consequent(std::move(consequent)) {}

Lr0Item::Lr0Item(std::string antecedent, std::vector<std::string> consequent,
                 unsigned int dot)
    : antecedent(std::move(antecedent)), consequent(std::move(consequent)),
      dot(dot) {}

std::string Lr0Item::nextToDot() const {
    if (dot == consequent.size()) {
        return symbol_table::EPSILON_;
    }
    return consequent[dot];
}

void Lr0Item::advanceDot() {
    dot++;
}

void Lr0Item::printItem() const {
    std::cout << "[ " << antecedent << " -> ";
    for (unsigned int i = 0; i < consequent.size(); ++i) {
        if (i == dot) {
            std::cout << "<*> ";
        }
        std::cout << consequent[i] << " ";
    }
    if (dot == consequent.size()) {
        std::cout << "<*>";
    }
    std::cout << "]";
}