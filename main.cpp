// Actual coding on main has not started yet, code in this file in it's current state is all temporary testing stuff

#include "data_structs.hpp"
#include "moving.hpp"
#include "search.hpp"

int main() {
    board board_;
    move output_move;
    short promote_decision;
    bool stale_or_checkmated;
    init_board(&board_);
    // search_for_best_move(&board_, &output_move, &promote_decision, 3, &stale_or_checkmated);
    // std::cout << output_move.square_from[0] << "\n" << output_move.square_from[1] << "\n" << output_move.square_to[0] << "\n" << output_move.square_to[1] << "\n";
    // // move m1 = {{4, 0}, {0, 6}};
    // // move m2 = {{1, 1}, {1, 3}};
    // // move m3 = {{0, 3}, {1, 2}};
    // // make_move(&m1, &board_);
    // // make_move(&m2, &board_);
    // // make_move(&m3, &board_);
    check_and_pin_feedback feedback;
    // // reset_recent_move(&board_);
    // // reset_recent_move(&board_);
    // // reset_recent_move(&board_);
    // // printf("%d", *std::begin(feedback.valid_squares));
    // // printf("%s", feedback.valid_squares.count(7) ? "7" : "nah");
    // // printf("%s", feedback.valid_squares.count(0) ? "0" : "nah");
    // // print_board(&board_);
    short x0;
    short x1;
    short x2;
    short x3;
    move move_;
    print_board(&board_);
    while (true) {
        is_checked_and_identify_pins_and_freedoms(&board_, board_.white_to_move, &feedback);
        std::cin >> x0;
        std::cin >> x1;
        std::cin >> x2;
        std::cin >> x3;
        move_.square_from[0] = x0;
        move_.square_to[0] = x2;
        move_.square_from[1] = x1;
        move_.square_to[1] = x3;
        if (valid_move(&move_, &board_, &feedback)) {
            make_move(&move_, &board_);
            print_board(&board_);
            search_for_best_move(&board_, &output_move, &promote_decision, 1, &stale_or_checkmated);
            make_move(&output_move, &board_, promote_decision);
            print_board(&board_);
        } else {
            printf("illegal\n");
        };
    };
    return 0;
};