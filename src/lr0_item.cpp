#include <boost/container_hash/hash.hpp>
#include <boost/functional/hash.hpp>
#include <cstddef>
#include <functional>
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

bool Lr0Item::operator==(const Lr0Item& other) const {
    return antecedent == other.antecedent && consequent == other.consequent &&
           dot == other.dot;
}

namespace std {
std::size_t hash<Lr0Item>::operator()(const Lr0Item& item) const {
    std::size_t seed = 0;

    seed ^= std::hash<std::string>()(item.antecedent) + 0x9e3779b9 +
            (seed << 6) + (seed >> 2);
    for (const auto& str : item.consequent) {
        seed ^= std::hash<std::string>()(str) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
    }
    seed ^= std::hash<int>()(item.dot) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}
} // namespace std
