#pragma once

#include <string>
#include <vector>

struct Lr0Item {
    std::string              antecedent;
    std::vector<std::string> consequent;
    unsigned int             dot = 0;

    Lr0Item(std::string antecedent, std::vector<std::string> consequent);
    Lr0Item(std::string antecedent, std::vector<std::string> consequent,
            unsigned int dot);
    std::string nextToDot() const;
    void        printItem() const;
    void        advanceDot();
    bool        isComplete() const;
    bool        operator==(const Lr0Item& other) const;
};

namespace std {
template <> struct hash<Lr0Item> {
    size_t operator()(const Lr0Item& item) const;
};
} // namespace std
