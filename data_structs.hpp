#ifndef data_structs
#define data_structs

#include <unordered_map>
#include "utils.hpp"

struct move_internal {
    char square_from[2];
    char square_to[2];
    char taken_piece; // Edge case: this is defined differently for pawns captured by en passant
};

const char en_passant_event = 31;
const char castle_event = 32;

struct move {
    char square_from[2];
    char square_to[2];
};

struct board {
    char piece_positions[8][8]; // The piece on each square of the current board
    char flags; // Specifies if it's still possible to  castle and if en passant is possible this turn
    char en_passant_column; // If en passant is possible, specifies the column the pawn to be taken is on
    int alpha; // Alpha value for alpha-beta pruning
    int beta; // Beta value for alpha-beta pruning
    move_internal * trace; // The moves that occurred in the game until now
    short move_trace_buffer_len; // The length of the buffer to hold moves
    short move; // The actual index of the most recent move
    char turns_since_capture_or_advance; // Tracks turns since pawn advance of capture for 50 move rule
    std::unordered_map<std::size_t, char> * past_positions; // Stores previous positions for the 3 check rule
};

const char white_castle_short = 1;
const char white_castle_long = 2;
const char white_can_en_passant = 4;
const char black_castle_short = 8;
const char black_castle_long = 16;
const char black_can_en_passant = 32;

const char white_king_catle_row = 0;
const char black_king_castle_row = 7;
const char white_pawn_double_advance_row = 1;
const char white_pawn_double_advance_row = 6;
const char king_init_col = 4;
const char king_castle_long_col = 2;
const char king_castle_short_col = 6;

const char empty_square = 0;

const char white_pawn = 11;
const char white_rook = 12;
const char white_knight = 13;
const char white_bishop = 14;
const char white_king = 15;
const char white_queen = 16;

const char black_pawn = 21;
const char black_rook = 22;
const char black_knight = 23;
const char black_bishop = 24;
const char black_king = 25;
const char black_queen = 26;

#endif