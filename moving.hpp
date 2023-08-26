#ifndef moving
#define moving

#include "data_structs.hpp"
#include "utils.hpp"
#include <cmath>

void init_board(board * unset) {
    unset->move = 0;
    unset->drawn = false;
    unset->white_to_move = true;
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

void make_move(move * move_, board * board_) { // Does not check legality, please check move is legal before calling this to avoid undefined behaviour
    char from_piece = board_->piece_positions[move_->square_from[0]][move_->square_from[1]];
    char to_piece = board_->piece_positions[move_->square_to[0]][move_->square_to[1]];
    move_internal current_move = {
        square_from: {move_->square_from[0], move_->square_from[1]},
        square_to: {move_->square_to[0], move_->square_to[1]},
        taken_piece: 0,
        flags: 0
    };
    if (board_->move == 0) {
        current_move.flags = white_castle_long | white_castle_long | black_castle_long | black_castle_short;
    } else {
        current_move.flags = (white_castle_long | white_castle_long | black_castle_long | black_castle_short) & board_->trace[board_->move - 1].flags;
    };
    if (from_piece == white_pawn || from_piece == black_pawn || to_piece != empty_square) {
        current_move.turns_since_capture_or_advance = 0;
    } else if (board_->move == 0) {
        current_move.turns_since_capture_or_advance = 1;
    } else {
        current_move.turns_since_capture_or_advance = board_->trace[board_->move - 1].turns_since_capture_or_advance + 1;
    };
    if (from_piece == white_king) {
        current_move.flags = current_move.flags ^ (white_castle_long | white_castle_short);
    };
    if (from_piece == black_king) {
        current_move.flags = current_move.flags ^ (black_castle_long | black_castle_short);
    };
    if (from_piece == white_king && move_->square_from[1] == white_king_castle_row && move_->square_from[0] == king_init_col) {
        if (move_->square_to[0] == king_castle_long_col || move_->square_to[0] == king_castle_short_col) {
            current_move.taken_piece = castle_event;
            board_->piece_positions[move_->square_from[0]][white_king_castle_row] = empty_square;
            board_->piece_positions[move_->square_to[0]][white_king_castle_row] = white_king;
            if (move_->square_to[0] == king_castle_long_col) {
                board_->piece_positions[rook_castle_long_col][white_king_castle_row] = white_rook;
                board_->piece_positions[rook_castle_long_col - 3][white_king_castle_row] = empty_square;
            } else {
                board_->piece_positions[rook_castle_short_col][white_king_castle_row] = white_rook;
                board_->piece_positions[rook_castle_short_col + 2][white_king_castle_row] = empty_square;
            };
        };
    };
    if (from_piece == black_king && move_->square_from[1] == black_king_castle_row && move_->square_from[0] == king_init_col) {
        if (move_->square_to[0] == king_castle_long_col || move_->square_to[0] == king_castle_short_col) {
            current_move.taken_piece = castle_event;
            board_->piece_positions[move_->square_from[0]][black_king_castle_row] = empty_square;
            board_->piece_positions[move_->square_to[0]][black_king_castle_row] = black_king;
            if (move_->square_to[0] == king_castle_long_col) {
                board_->piece_positions[rook_castle_long_col][black_king_castle_row] = black_rook;
                board_->piece_positions[long_castle_to_clean_col][black_king_castle_row] = empty_square;
            } else {
                board_->piece_positions[rook_castle_short_col][black_king_castle_row] = black_rook;
                board_->piece_positions[short_castle_to_clean_col][black_king_castle_row] = empty_square;
            };
        };
    };
    if (current_move.taken_piece == 0) {
        if ((from_piece == white_pawn || from_piece == black_pawn) && abs(current_move.square_from[1] - current_move.square_to[1]) == 2) {
            current_move.flags = current_move.flags | can_en_passant;
        };
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
    board_->white_to_move = !board_->white_to_move;
    size_t hash = hash_position(&(board_->piece_positions[0][0]));
    if (board_->past_positions.find(hash) != board_->past_positions.end()) {
        board_->past_positions.find(hash)->second++;
        if (board_->past_positions.find(hash)->second >= 3) {
            board_->drawn = true;
        };
    } else {
        board_->past_positions.insert(std::make_pair(hash, 1));
    };
};

void reset_recent_move(board * board_) {
    board_->move--;
    move_internal move_to_undo = board_->trace[board_->move];
    board_->white_to_move = !board_->white_to_move;
    size_t hash = hash_position(&(board_->piece_positions[0][0]));
    if (board_->past_positions.find(hash)->second >= 2) {
        board_->past_positions.find(hash)->second--;
    } else {
        board_->past_positions.erase(hash);
    };
    if (move_to_undo.taken_piece == castle_event) {
        if (move_to_undo.square_from[1] == white_king_castle_row) {
            board_->piece_positions[king_init_col][white_king_castle_row] = white_king;
            if (move_to_undo.square_to[0] == king_castle_long_col) {
                board_->piece_positions[king_castle_long_col][white_king_castle_row] = empty_square;
                board_->piece_positions[rook_castle_long_col][white_king_castle_row] = empty_square;
                board_->piece_positions[long_castle_to_clean_col][white_king_castle_row] = white_rook;
            } else {
                board_->piece_positions[king_castle_short_col][white_king_castle_row] = empty_square;
                board_->piece_positions[rook_castle_short_col][white_king_castle_row] = empty_square;
                board_->piece_positions[short_castle_to_clean_col][white_king_castle_row] = white_rook;
            };
        } else {
            board_->piece_positions[king_init_col][black_king_castle_row] = black_king;
            if (move_to_undo.square_to[0] == king_castle_long_col) {
                board_->piece_positions[king_castle_long_col][black_king_castle_row] = empty_square;
                board_->piece_positions[rook_castle_long_col][black_king_castle_row] = empty_square;
                board_->piece_positions[long_castle_to_clean_col][black_king_castle_row] = black_rook;
            } else {
                board_->piece_positions[king_castle_short_col][black_king_castle_row] = empty_square;
                board_->piece_positions[rook_castle_short_col][black_king_castle_row] = empty_square;
                board_->piece_positions[short_castle_to_clean_col][black_king_castle_row] = black_rook;
            };
        };
    } else {
        const char to_piece = board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]];
        if (move_to_undo.taken_piece == en_passant_event) {
            if (to_piece == white_pawn) {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1] - 1] = black_pawn;
            } else {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1] + 1] = white_pawn;
            };
            board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]] = empty_square;
        } else {
            board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]] = move_to_undo.taken_piece;
        }
        board_->piece_positions[move_to_undo.square_from[0]][move_to_undo.square_from[1]] = to_piece;
    };
};

#endif