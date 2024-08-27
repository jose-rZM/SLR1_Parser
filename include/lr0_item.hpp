#pragma once

#include <string>
#include <vector>

struct Lr0Item {
    std::string              antecedent;
    std::vector<std::string> consequent;
    int                      dot = -1;

    Lr0Item(std::string antecedent, std::vector<std::string>& consequent);
    std::string nextToDot() const;
    void        advanceDot();
};