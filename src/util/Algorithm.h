#pragma once 

#include <unordered_map>

namespace ste {
    template<typename _Key, typename _Value, class UnaryFunction>
    void unordered_map_erase_if(std::unordered_map<_Key, _Value> &m, UnaryFunction f) {
        for ( auto it = m.begin(); it != m.end(); ) {
            f(*it) ? it = m.erase(it) : it++;
        }
    }
}
