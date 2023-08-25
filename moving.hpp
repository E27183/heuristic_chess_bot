#ifndef moving
#define moving

#include "data_structs.hpp"
#include "utils.hpp"
#include <cmath>

void init_board(board * unset) {
    unset->flags = white_castle_long | white_castle_long | black_castle_long | black_castle_short;
    unset->turns_since_capture_or_advance = 0;
    unset->move = 0;
    unset->drawn = false;
    unset->beta = __FLT_MAX__;
    unset->alpha = -__FLT_MAX__;
    char init[8][8] = {
        {white_rook, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_rook},
        {white_knight, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_knight},
        {white_bishop, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_bishop},
        {white_queen, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_queen},
        {white_king, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_king},
        {white_bishop, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_bishop},
        {white_knight, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_knight},
        {white_rook, white_pawn, empty_square, empty_square, empty_square, empty_square, black_pawn, black_rook}
    };
    for (int i = 0; i < 8; i++) { // Must do this manually as std::copy is blocked by struct memory protection
        for (int j = 0; j < 8; j++) {
            unset->piece_positions[i][j] = init[i][j];
        };
    };
};

void make_move(move * move_, board * board_) { // Does not check legality, please check legal before calling this
    char from_piece = board_->piece_positions[move_->square_from[0]][move_->square_from[1]];
    char to_piece = board_->piece_positions[move_->square_to[0]][move_->square_to[1]];
    if (from_piece == white_pawn || from_piece == black_pawn || to_piece != empty_square) {
        board_->turns_since_capture_or_advance = 0;
    } else {
        board_->turns_since_capture_or_advance++;
    };
    
    move_internal current_move = {
        square_from: *(move_->square_from),
        square_to: *(move_->square_to),
        taken_piece: 0
    };
    if (from_piece == white_king) {
        board_->flags = board_->flags ^ (white_castle_long | white_castle_short);
    };
    if (from_piece == black_king) {
        board_->flags = board_->flags ^ (black_castle_long | black_castle_short);
    };
    if (from_piece == white_king && move_->square_from[0] == white_king_castle_row && move_->square_from[1] == king_init_col) {
        if (move_->square_to[1] == king_castle_long_col || move_->square_to[1] == king_castle_short_col) {
            current_move.taken_piece = 32;
            board_->piece_positions[white_king_castle_row][move_->square_from[1]] = empty_square;
            board_->piece_positions[white_king_castle_row][move_->square_to[1]] = white_king;
            if (move_->square_to[1] == king_castle_long_col) {
                board_->piece_positions[white_king_castle_row][rook_castle_long_col] = white_rook;
            } else {
                board_->piece_positions[white_king_castle_row][rook_castle_short_col] = white_rook;
            };
        };
    };
    if (from_piece == black_king && move_->square_from[0] == black_king_castle_row && move_->square_from[1] == king_init_col) {
        if (move_->square_to[1] == king_castle_long_col || move_->square_to[1] == king_castle_short_col) {
            current_move.taken_piece = 32;
            board_->piece_positions[black_king_castle_row][move_->square_from[1]] = empty_square;
            board_->piece_positions[black_king_castle_row][move_->square_to[1]] = black_king;
            if (move_->square_to[1] == king_castle_long_col) {
                board_->piece_positions[black_king_castle_row][rook_castle_long_col] = black_rook;
            } else {
                board_->piece_positions[black_king_castle_row][rook_castle_short_col] = black_rook;
            };
        };
    };
    if (current_move.taken_piece == 0) {
        if ((from_piece == white_pawn || from_piece == black_pawn) && abs(current_move.square_from[1] - current_move.square_to[1]) == 2) {
            board_->en_passant_column = current_move.square_from[0];
            board_->flags = board_->flags | can_en_passant;
        } else {
            board_->flags = board_->flags ^ can_en_passant;
        };
        //0 col, 1 row
        board_->piece_positions[move_->square_from[0]][move_->square_from[1]] = empty_square;
        board_->piece_positions[move_->square_to[0]][move_->square_to[1]] = from_piece;
        if ((from_piece == white_pawn || from_piece == black_pawn) && current_move.square_from[0] != current_move.square_to[0] && to_piece == empty_square) {
            current_move.taken_piece = en_passant_event;
            if (from_piece == white_pawn) {
                board_->piece_positions[move_->square_to[0]][move_->square_to[1] - 1] = empty_square;
            } else {
                board_->piece_positions[move_->square_to[0]][move_->square_to[1] + 1] = empty_square;
            };
        } else {
            current_move.taken_piece = to_piece;
        };
    };
    board_->trace[board_->move] = current_move;
    board_->move++;
    size_t hash = hash_position(&(board_->piece_positions[0][0]));
    if (board_->past_positions->find(hash) != board_->past_positions->end()) {
        board_->past_positions->find(hash)->second++;
        if (board_->past_positions->find(hash)->second >= 3) {
            board_->drawn = true;
        };
    } else {
        board_->past_positions->insert(std::make_pair(hash, 1));
    };
};

#endif