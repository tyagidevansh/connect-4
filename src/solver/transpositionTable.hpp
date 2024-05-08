#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <vector>
#include <cstring>
#include <cassert>
#include <cstdint>

class TranspositionTable {
    private:
    
    struct Entry {
        uint64_t key: 56;
        uint8_t val;
    };

    std::vector<Entry> T;

    unsigned int index(uint64_t key) const {
        return key%T.size(); 
    }

    public:

    TranspositionTable(unsigned int size) : T(size) {
        assert(size > 0);
    }

    void reset() {
        memset(&T[0], 0, T.size()*sizeof(Entry));
    }

    void put(uint64_t key, uint8_t val) {
        assert(key < (1LL << 56));
        unsigned int i = index(key);
        T[i].key = key;
        T[i].val = val;
    }

    uint8_t get(uint64_t key) const {
        assert(key < (1LL << 56));
        unsigned int i = index(key);
        if (T[i].key == key)
            return T[i].val;
        else   
            return 0;
    }
};

#endif // !TRANSPOSITION_TABLE_HPP