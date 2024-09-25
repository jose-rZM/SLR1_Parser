
#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <functional>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include "lr0_item.hpp"

struct state {
    std::unordered_set<Lr0Item> items;
    unsigned int id;
};

namespace std {
template <> struct hash<state> {
    size_t operator()(const state& st) const {
        size_t seed = 0;

        boost::hash_combine(seed, boost::hash_range(st.items.begin(), st.items.end()));
        boost::hash_combine(seed, st.id);

        return seed;
    }
};
}
