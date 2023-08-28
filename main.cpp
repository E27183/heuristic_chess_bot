// Actual coding on main has not started yet, code in this file in it's current state is all temporary testing stuff

#include "data_structs.hpp"
#include "moving.hpp"

int main() {
    board board_;
    init_board(&board_);
    move m1 = {{4, 0}, {0, 6}};
    // move m2 = {{1, 1}, {1, 3}};
    // move m3 = {{0, 3}, {1, 2}};
    make_move(&m1, &board_);
    // make_move(&m2, &board_);
    // make_move(&m3, &board_);
    check_and_pin_feedback feedback;
    is_checked_and_identify_pins_and_freedoms(&board_, !board_.white_to_move, &feedback);
    // reset_recent_move(&board_);
    // reset_recent_move(&board_);
    // reset_recent_move(&board_);
    // printf("%d", *std::begin(feedback.valid_squares));
    // printf("%s", feedback.valid_squares.count(7) ? "7" : "nah");
    // printf("%s", feedback.valid_squares.count(0) ? "0" : "nah");
    // print_board(&board_);
    return 0;
};