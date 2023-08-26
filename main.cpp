// Actual coding on main has not started yet, code in this file in it's current state is all temporary testing stuff

#include "data_structs.hpp"
#include "moving.hpp"

int main() {
    board board_;
    init_board(&board_);
    move m1 = {{0, 6}, {0, 4}};
    move m2 = {{1, 1}, {1, 3}};
    move m3 = {{0, 3}, {1, 2}};
    make_move(&m1, &board_);
    make_move(&m2, &board_);
    make_move(&m3, &board_);
    reset_recent_move(&board_);
    reset_recent_move(&board_);
    reset_recent_move(&board_);
    print_board(&board_);
    return 0;
};