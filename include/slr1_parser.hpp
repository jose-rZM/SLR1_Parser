#include <map>
#include <string>
#include <unordered_set>

#include "grammar.hpp"
#include "lr0_item.hpp"
#include "state.hpp"

class SLR1Parser {
  public:
    enum class Action { Shift, Reduce, Accept, Empty };

    using action_table = std::map<unsigned int, std::map<std::string, Action>>;
    using transition_table =
        std::map<unsigned int, std::map<std::string, unsigned int>>;
    SLR1Parser(grammar gr, std::string text_file);
    SLR1Parser(const std::string& grammar_file, std::string text_file);
    explicit SLR1Parser(const std::string& grammar_file);
    std::unordered_set<Lr0Item> allItems() const;
    bool                        parse() const;
    void                        debugStates() const;
    void                        debugActions() const;
    void                        debugTable() const;
    void closure(std::unordered_set<Lr0Item>& items) const;
    void closureUtil(std::unordered_set<Lr0Item>& items, unsigned int size,
                     std::unordered_set<std::string>& visited) const;
    void solveLRConflicts(const state& st);
    /**
     *
     * @param rule
     * @return set header symbols for the given rule
     * TODO: if grammar consists only in one non terminal (apart the axiom), it
     * could end with an infinite loop. For example: A -> A & A, A -> ( A ). The
     * grammar obviously is not LL1, but this will provoke an infinite loop.
     */
    std::unordered_set<std::string>
    first(const std::vector<std::string>& rule) const;
    /**
     * Compute the first sets of all non terminal symbols following the least
     * fixed point solution
     */
    void compute_first_sets();
    /**
     *
     * @param arg symbol to calculate next symbols for
     * @return Set of next symbols for the given arg
     */
    std::unordered_set<std::string> follow(const std::string& arg) const;
    /**
     *
     * @param arg
     * @param visited symbols (avoid infinite recursion)
     * @param next_symbols next symbols accumulated
     */
    void follow_util(const std::string&               arg,
                     std::unordered_set<std::string>& visited,
                     std::unordered_set<std::string>& next_symbols) const;

    void makeInitialState();
    void make_parser();

    grammar     gr_;
    std::string grammar_file_;
    std::string text_file_;
    std::unordered_map<std::string, std::unordered_set<std::string>> first_sets;
    action_table                                                     actions_;
    transition_table          transitions_;
    std::unordered_set<state> states_;
};
