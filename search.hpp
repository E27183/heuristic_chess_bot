#ifndef search
#define search

#include <thread>
#include <mutex>
#include <vector>
#include "data_structs.hpp"

const unsigned int numCores = std::thread::hardware_concurrency();

void search_for_best_move(board * board_, move * output_move, short * output_promotion_decision, short depth) {
    //TODO
};

#endif