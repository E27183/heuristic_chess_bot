#ifndef moving
#define moving

#include "data_structs.hpp"

void init_board(board * unset) { // Move trace  and move trace buffer len MUST be defined in main scope
    unset->flags = white_castle_long | white_castle_long | black_castle_long | black_castle_short;
    unset->turns_since_capture_or_advance = 0;
};

void make_move(move * move_, board * board_) { //Check legal before committing to this
    char from_piece = board_->piece_positions[move_->square_from[0]][move_->square_from[1]];
    if (from_piece == white_king) {
        board_->flags = board_->flags ^ (white_castle_long | white_castle_short);
    };
    if (from_piece == black_king) {
        board_->flags = board_->flags ^ (black_castle_long | black_castle_short);
    };
    if (from_piece == white_king && move_->square_from[0] == white_king_catle_row && move_->square_from[1] == king_init_col) {
        if (move_->square_to[1] == king_castle_long_col) {

        };
    };
};

#endif