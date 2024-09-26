
#include "lr0_item.hpp"
#include <boost/container_hash/hash.hpp>
#include <boost/functional/hash.hpp>
#include <cstddef>
#include <functional>
#include <unordered_set>

struct state {
    std::unordered_set<Lr0Item> items;
    unsigned int                id;

    bool operator==(const state& other) const { return other.items == items; }
};

namespace std {
template <> struct hash<state> {
    size_t operator()(const state& st) const {
        size_t seed = 0;

        for (const auto& item : st.items) {
            seed ^= std::hash<Lr0Item>()(item) + 0x9e3779b9 + (seed << 6) +
                    (seed >> 2);
        }

        return seed;
    }
};
} // namespace std
