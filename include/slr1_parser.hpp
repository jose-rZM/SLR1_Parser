#include <string>
#include <unordered_set>

#include "grammar.hpp"

class SLR1Parser {
  public:
    SLR1Parser(grammar gr, std::string text_file);
    SLR1Parser(const std::string& grammar_file, std::string text_file);
    explicit SLR1Parser(const std::string& grammar_file);
    bool parse();

  private:
    /**
     *
     * @param rule
     * @return set header symbols for the given rule
     * TODO: if grammar consists only in one non terminal (apart the axiom), it
     * could end with an infinite loop. For example: A -> A & A, A -> ( A ). The
     * grammar obviously is not LL1, but this will provoke an infinite loop.
     */
    std::unordered_set<std::string>
    header(const std::vector<std::string>& rule);
    /**
     *
     * @param arg symbol to calculate next symbols for
     * @return Set of next symbols for the given arg
     */
    std::unordered_set<std::string> follow(const std::string& arg);
    /**
     *
     * @param arg
     * @param visited symbols (avoid infinite recursion)
     * @param next_symbols next symbols accumulated
     */
    void follow_util(const std::string&               arg,
                     std::unordered_set<std::string>& visited,
                     std::unordered_set<std::string>& next_symbols);

    grammar     gr_;
    std::string grammar_file_;
    std::string text_file_;
};