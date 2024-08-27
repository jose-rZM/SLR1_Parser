#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../include/grammar.hpp"
#include "../include/slr1_parser.hpp"
#include "../include/symbol_table.hpp"

SLR1Parser::SLR1Parser(grammar gr, std::string text_file)
    : gr_(std::move(gr)), text_file_(std::move(text_file)) {}

SLR1Parser::SLR1Parser(const std::string& grammar_file, std::string text_file)
    : gr_(grammar_file), text_file_(std::move(text_file)) {}

SLR1Parser::SLR1Parser(const std::string& grammar_file) : gr_(grammar_file) {}

std::unordered_set<std::string>
SLR1Parser::header(const std::vector<std::string>& rule) {
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

std::unordered_set<std::string> SLR1Parser::follow(const std::string& arg) {
    std::unordered_set<std::string> next_symbols;
    std::unordered_set<std::string> visited;
    follow_util(arg, visited, next_symbols);
    if (next_symbols.find(symbol_table::EPSILON_) != next_symbols.end()) {
        next_symbols.erase(symbol_table::EPSILON_);
    }
    return next_symbols;
}

void SLR1Parser::follow_util(const std::string&               arg,
                             std::unordered_set<std::string>& visited,
                             std::unordered_set<std::string>& next_symbols) {
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