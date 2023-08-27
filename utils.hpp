#ifndef utils
#define utils

#include <functional>

std::size_t hash_position(short * board) {
    std::size_t hash = std::hash<short>{}(*board);
    for (int i = 1; i < 64; i++) {
        hash = hash ^ std::hash<short>{}(*board + i);
    };
    return hash;
};

#endif