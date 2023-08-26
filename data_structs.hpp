#ifndef data_structs
#define data_structs

#include <unordered_map>
#include <iostream>
#include "utils.hpp"

struct move_internal {
    char square_from[2];
    char square_to[2];
    char taken_piece; // Edge case: this is defined differently for pawns captured by en passant
    char turns_since_capture_or_advance;  // Tracks turns since pawn advance of capture for 50 move rule
    char flags; // Specifies if it's still possible to  castle and if en passant is possible this turn
};

const char en_passant_event = 31;
const char castle_event = 32;

struct move {
    char square_from[2];
    char square_to[2];
};

struct board {
    char piece_positions[8][8]; // The piece on each square of the current board
    bool white_to_move;
    move_internal trace[2048]; // The moves that occurred in the game until now
    short move; // Moves the have occurred
    bool drawn; // Easy identifier for the game being drawn due to reaching the same position for the third time
    std::unordered_map<std::size_t, char> past_positions; // Stores previous positions for the 3 check rule
};

const char white_castle_short = 1;
const char white_castle_long = 2;
const char can_en_passant = 4;
const char black_castle_short = 8;
const char black_castle_long = 16;

const char white_king_castle_row = 0;
const char long_castle_to_clean_col = 0;
const char short_castle_to_clean_col = 7;
const char black_king_castle_row = 7;
const char white_pawn_double_advance_row = 1;
const char black_pawn_double_advance_row = 6;
const char king_init_col = 4;
const char king_castle_long_col = 2;
const char king_castle_short_col = 6;
const char rook_castle_long_col = 3;
const char rook_castle_short_col = 5;

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

char internal_to_printed(char in) {
    switch (in) {
        case empty_square:
            return ' ';
        case white_pawn:
            return 'p';
        case black_pawn:
            return 'P';
        case white_rook:
            return 'r';
        case black_rook:
            return 'R';
        case white_knight:
            return 'n';
        case black_knight:
            return 'N';
        case white_bishop:
            return 'b';
        case black_bishop:
            return 'B';
        case white_king:
            return 'k';
        case black_king:
            return 'K';
        case white_queen:
            return 'q';
        case black_queen:
            return 'Q';    
        default:
            return '?';
    };
};

void print_board(board * board_) {
    printf("flags: %d\n"
    "white move: %d\n"
    "move: %d\n"
    "50_move_rule: %d\n"
    "moves: [", board_->move == 0 ? 0 : board_->trace[board_->move - 1].turns_since_capture_or_advance, board_->white_to_move,
    board_->move, board_->move == 0 ? 0 : board_->trace[board_->move - 1].turns_since_capture_or_advance);
    for (int i = 0; i < board_->move; i++) {
        printf("%d-%d | %d-%d | %d, ", 
        board_->trace[i].square_from[0],
        board_->trace[i].square_from[1],
        board_->trace[i].square_to[0],
        board_->trace[i].square_to[1],
        board_->trace[i].taken_piece);
    };
    printf("]");
    for (int i = 7; i >= 0; i--) {
        printf("\n+-+-+-+-+-+-+-+-+\n|");
        for (int j = 0; j < 8; j++) {
            printf("%c|", internal_to_printed(board_->piece_positions[j][i]));
        };
    };
    printf("\n+-+-+-+-+-+-+-+-+\n");
};

#endif