
#include "lr0_item.hpp"
#include <boost/container_hash/hash.hpp>
#include <boost/functional/hash.hpp>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_set>

struct state {
    std::unordered_set<Lr0Item> items;
    unsigned int                id;

    bool operator==(const state& other) const { return other.items == items; }
};

namespace std {
template <> struct hash<state> {
    size_t operator()(const state& st) const {
        /*size_t seed = std::accumulate(
                st.items.begin(), st.items.end(), 0, [](size_t acc, const Lr0Item& item) {
                    return acc ^ (std::hash<Lr0Item>()(item) + 0x9e3779b9 + (acc << 6) + (acc >> 2));
                }
            );*/
        size_t seed = std::accumulate(
                st.items.begin(), st.items.end(), 0, [](size_t acc, const Lr0Item& item) {
                    return acc ^ (std::hash<Lr0Item>()(item));
                }
            );
        return seed;
    }
};
} // namespace std
