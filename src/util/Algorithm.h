#pragma once 

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace ste {
    template<typename _Key, typename _Value, class UnaryFunction>
    void unordered_map_erase_if(std::unordered_map<_Key, _Value> &m, UnaryFunction &f) {
        for ( auto it = m.begin(); it != m.end(); ) {
            f(*it) ? it = m.erase(it) : it++;
        }
    }

    template<typename T, class UnaryFunction>
    void vector_erase_if(std::vector<T> &v, UnaryFunction f) {
        v.erase(std::remove_if(begin(v), end(v), f), end(v));
    }

    template<typename T>
    void vector_erase(std::vector<T> &v, const T &item) {
        v.erase(std::remove(begin(v), end(v), item));
    }
}

namespace stdr {
    template<class _Collection, class UnaryFunction> 
    UnaryFunction for_each(_Collection &c, UnaryFunction f) {
        std::for_each(begin(c), end(c), f);
    }
}
