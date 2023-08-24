#ifndef utils
#define utils

#include <functional>

std::size_t hash_position(char * board) {
    std::size_t hash = std::hash<char>{}(*board);
    for (int i = 1; i < 64; i++) {
        hash = hash ^ (std::hash<char>{}(*board + i) << 1);
    };
    return hash;
};

#endif