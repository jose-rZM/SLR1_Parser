#include <string>
#include <utility>
#include <vector>

#include "../include/lr0_item.hpp"
#include "../include/symbol_table.hpp"

Lr0Item::Lr0Item(std::string antecedent, std::vector<std::string>& consequent)
    : antecedent(std::move(antecedent)), consequent(std::move(consequent)) {}

std::string Lr0Item::nextToDot() const {
    if (dot == consequent.size()) {
        return symbol_table::EPSILON_;
    }
    return consequent[dot + 1];
}

void Lr0Item::advanceDot() {
    dot++;
}