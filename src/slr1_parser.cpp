#include "../include/symbol_table.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../include/grammar.hpp"
#include "../include/slr1_parser.hpp"

SLR1Parser::SLR1Parser(grammar gr, std::string text_file)
    : gr_(std::move(gr)), text_file_(std::move(text_file)) {}

SLR1Parser::SLR1Parser(const std::string& grammar_file, std::string text_file)
    : gr_(grammar_file), text_file_(std::move(text_file)) {}

SLR1Parser::SLR1Parser(const std::string& grammar_file) : gr_(grammar_file) {}

std::unordered_set<Lr0Item> SLR1Parser::allItems() const {
    std::unordered_set<Lr0Item> items;
    for (const auto& rule : gr_.g_) {
        for (const auto& production : rule.second) {
            for (unsigned int i = 0; i <= production.size(); ++i)
                items.insert({rule.first, production, i});
        }
    }
    return items;
}

void SLR1Parser::debugStates() const {
    for (const auto& s : states_) {
        std::cout << "State ID: " << s.id << std::endl;
        std::cout << "Items:" << std::endl;
        for (const auto& item : s.items) {
            std::cout << "\t";
            item.printItem();
            std::cout << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }
}

void SLR1Parser::debugActions() const {
    std::map<std::string, bool> columns;

    for (const auto& row : actions_) {
        for (const auto& cell : row.second) {
            columns[cell.first] = true;
        }
    }

    std::cout << std::setw(10) << "ACTIONS |";
    for (const auto& col : columns) {
        std::cout << std::setw(10) << col.first << " |";
    }
    std::cout << std::endl;

    std::cout << std::string(10 + (columns.size() * 13), '-') << std::endl;

    for (unsigned int state = 0; state < states_.size(); ++state) {
        std::cout << std::setw(10) << state << " |";

        auto rowIt = actions_.find(state);
        if (rowIt != actions_.end() && !rowIt->second.empty()) {
            for (const auto& col : columns) {
                auto cellIt = rowIt->second.find(col.first);
                if (cellIt != rowIt->second.end()) {
                    switch (cellIt->second) {
                    case Action::Accept:
                        std::cout << std::setw(10) << "A" << " |";
                        break;
                    case Action::Reduce:
                        std::cout << std::setw(10) << "R" << " |";
                        break;
                    case Action::Shift:
                        std::cout << std::setw(10) << "S" << " |";
                        break;
                    default:
                        std::cout << std::setw(10) << "-" << " |";
                        break;
                    }
                } else {
                    std::cout << std::setw(10) << "-" << " |";
                }
            }
        } else {
            for (const auto& _ : columns) {
                std::cout << std::setw(10) << "-" << " |";
            }
        }
        std::cout << std::endl;
    }
}

void SLR1Parser::debugTable() const {
    std::map<std::string, bool> columns;

    for (const auto& row : transitions_) {
        for (const auto& cell : row.second) {
            columns[cell.first] = true;
        }
    }

    std::cout << std::setw(10) << "TRANSITIONS |";
    for (const auto& col : columns) {
        std::cout << std::setw(10) << col.first << " |";
    }
    std::cout << std::endl;

    std::cout << std::string(10 + (columns.size() * 13), '-') << std::endl;

    for (unsigned int state = 0; state < states_.size(); ++state) {
        std::cout << std::setw(10) << state << " |";

        auto rowIt = transitions_.find(state);
        if (rowIt != transitions_.end() && !rowIt->second.empty()) {
            for (const auto& col : columns) {
                auto cellIt = rowIt->second.find(col.first);
                if (cellIt != rowIt->second.end()) {
                    std::cout << std::setw(10) << cellIt->second << " |";
                } else {
                    std::cout << std::setw(10) << "-" << " |";
                }
            }
        } else {
            for (const auto& _ : columns) {
                std::cout << std::setw(10) << "-" << " |";
            }
        }
        std::cout << std::endl;
    }
}

void SLR1Parser::makeInitialState() {
    state initial;
    initial.id = 0;
    for (const auto& rule : gr_.g_) {
        std::string antecedent = rule.first;
        for (const auto& production : rule.second) {
            initial.items.insert({antecedent, production, 0});
        }
    }
    states_.insert(initial);
}

void SLR1Parser::solveLRConflicts(const state& st) {
    for (const Lr0Item& item : st.items) {
        if (item.isComplete()) {
            if (item.antecedent == gr_.AXIOM_) {
                actions_[st.id][symbol_table::EOL_] = Action::Accept;
            } else {
                std::unordered_set<std::string> follows =
                    follow(item.antecedent);
                for (const std::string& sym : follows) {
                    actions_[st.id][sym] = Action::Reduce;
                }
            }
        } else {
            std::string nextToDot = item.nextToDot();
            if (symbol_table::is_terminal(item.nextToDot())) {
                actions_[st.id][item.nextToDot()] = Action::Shift;
            }
        }
    }
}

void SLR1Parser::make_parser() {
    makeInitialState();
    std::stack<unsigned int> pending;
    pending.push(0);
    unsigned int current = 0;
    size_t       i       = 1;

    do {
        std::unordered_set<std::string> nextSymbols;
        current = pending.top();
        pending.pop();
        auto it = std::find_if(
            states_.begin(), states_.end(),
            [current](const state& st) -> bool { return st.id == current; });
        if (it == states_.end()) {
            break;
        }
        const state& qi = *it;
        solveLRConflicts(qi);
        std::for_each(qi.items.begin(), qi.items.end(),
                      [&](const Lr0Item& item) -> void {
                          std::string next = item.nextToDot();
                          if (next != symbol_table::EPSILON_ &&
                              next != symbol_table::EOL_) {
                              nextSymbols.insert(next);
                          }
                      });
        for (const std::string& symbol : nextSymbols) {
            state newState;
            newState.id = i;
            for (const auto& item : qi.items) {
                if (item.nextToDot() == symbol) {
                    Lr0Item newItem = item;
                    newItem.advanceDot();
                    newState.items.insert(newItem);
                }
            }

            closure(newState.items);
            auto result = states_.insert(newState);
            std::map<std::string, unsigned int> column;

            if (result.second) {
                pending.push(i);
                if (transitions_.find(current) != transitions_.end()) {
                    transitions_[current].insert({symbol, i});
                } else {
                    std::map<std::string, unsigned int> column;
                    column.insert({symbol, i});
                    transitions_.insert({current, column});
                }
                ++i;
            } else {
                if (transitions_.find(current) != transitions_.end()) {
                    transitions_[current].insert({symbol, result.first->id});
                } else {
                    std::map<std::string, unsigned int> column;
                    column.insert({symbol, result.first->id});
                    transitions_.insert({current, column});
                }
            }
        }
        current++;
    } while (!pending.empty());
}

void SLR1Parser::closure(std::unordered_set<Lr0Item>& items) const {
    std::unordered_set<std::string> visited;
    closureUtil(items, items.size(), visited);
}

void SLR1Parser::closureUtil(std::unordered_set<Lr0Item>&     items,
                             unsigned int                     size,
                             std::unordered_set<std::string>& visited) const {
    std::unordered_set<Lr0Item> newItems;

    for (const auto& item : items) {
        std::string next = item.nextToDot();
        if (!symbol_table::is_terminal(next) &&
            std::find(visited.cbegin(), visited.cend(), next) ==
                visited.cend()) {
            const std::vector<production>& rules = gr_.g_.at(next);
            std::for_each(rules.begin(), rules.end(),
                          [&](const auto& rule) -> void {
                              newItems.insert({item.nextToDot(), rule, 0});
                          });
            visited.insert(next);
        }
    }
    items.insert(newItems.begin(), newItems.end());
    if (size != items.size())
        closureUtil(items, items.size(), visited);
}

std::unordered_set<std::string>
SLR1Parser::header(const std::vector<std::string>& rule) const {
    std::unordered_set<std::string>      current_header;
    std::stack<std::vector<std::string>> symbol_stack;
    symbol_stack.push(rule);

    while (!symbol_stack.empty()) {
        std::vector<std::string> current = symbol_stack.top();
        symbol_stack.pop();
        if (current[0] == symbol_table::EPSILON_) {
            current.erase(current.begin());
        }
        if (current.empty()) {
            current_header.insert(symbol_table::EPSILON_);
        } else if (symbol_table::is_terminal(current[0])) {
            current_header.insert(current[0]);
        } else {
            for (const std::vector<std::string>& prod : gr_.g_.at(current[0])) {
                std::vector<std::string> production;
                for (const std::string& sy : prod) {
                    production.push_back(sy);
                }
                // Add remaining symbols
                for (unsigned i = 1; i < current.size(); ++i) {
                    production.push_back(current[i]);
                }
                symbol_stack.push(production);
            }
        }
    }

    return current_header;
}

std::unordered_set<std::string>
SLR1Parser::follow(const std::string& arg) const {
    std::unordered_set<std::string> next_symbols;
    std::unordered_set<std::string> visited;
    follow_util(arg, visited, next_symbols);
    if (next_symbols.find(symbol_table::EPSILON_) != next_symbols.end()) {
        next_symbols.erase(symbol_table::EPSILON_);
    }
    return next_symbols;
}

void SLR1Parser::follow_util(
    const std::string& arg, std::unordered_set<std::string>& visited,
    std::unordered_set<std::string>& next_symbols) const {
    if (visited.find(arg) != visited.cend()) {
        return;
    }
    visited.insert(arg);
    std::vector<std::pair<const std::string, production>> rules{
        gr_.filter_rules_by_consequent(arg)};

    for (const std::pair<const std::string, production>& rule : rules) {
        // Next must be applied to all Arg symbols, for example
        // if arg: B; A -> BbBCB, next is applied three times
        auto it = rule.second.cbegin();
        while ((it = std::find(it, rule.second.cend(), arg)) !=
               rule.second.cend()) {
            auto next_it = std::next(it);
            if (next_it == rule.second.cend()) {
                follow_util(rule.first, visited, next_symbols);
            } else {
                next_symbols.merge(header({*next_it}));
            }
            it = std::next(it);
        }
    }
}
