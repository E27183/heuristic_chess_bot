#ifndef data_structs
#define data_structs

#include <unordered_map>
#include <iostream>
#include <unordered_set>

#include "utils.hpp"

short up_down = 1;
short left_right = 2;
short ascending = 3;
short descending = 4;

const short most_pins_possible = 8;

struct pinned_piece {
    short location[2]; // Location of the pinned piece
    short direction; // Direction it can still move in
};

struct check_and_pin_feedback {
    short checks; // If this is 2 or more, the number may not be accurate but enough to determine the king must move
    short checker_location[2]; // Only defined if there is exactly one check
    std::unordered_set<short> valid_squares; // Squares that can be moved to to block the check, defined as 64 * row + col, or row << 6 + col
    pinned_piece pinned_pieces[most_pins_possible]; // All pieces pinned
    short pinned_pieces_count; // Amount of pieces pinned
};

struct move_internal {
    short square_from[2];
    short square_to[2];
    short taken_piece; // Edge case: this is defined differently for pawns captured by en passant
    short turns_since_capture_or_advance;  // Tracks turns since pawn advance of capture for 50 move rule
    short flags; // Specifies if it's still possible to  castle and if en passant is possible this turn
};

const short en_passant_event = 31;
const short castle_event = 32;
const short promotion_event = 33; // Don't need by case promotion as info is more accessible from move trace + boards

struct move {
    short square_from[2];
    short square_to[2];
};

struct board {
    short piece_positions[8][8]; // The piece on each square of the current board
    bool white_to_move;
    move_internal trace[2048]; // The moves that occurred in the game until now
    short move; // Moves the have occurred
    bool drawn; // Easy identifier for the game being drawn due to reaching the same position for the third time
    std::unordered_map<std::size_t, short> past_positions; // Stores previous positions for the 3 check rule
};

const short white_castle_short = 1;
const short white_castle_long = 2;
const short can_en_passant = 4;
const short black_castle_short = 8;
const short black_castle_long = 16;

const short white_king_castle_row = 0, black_promote_row = 0, long_castle_to_clean_col = 0;
const short black_king_castle_row = 7, white_promote_row = 7, short_castle_to_clean_col = 7;
const short white_pawn_double_advance_row = 1;
const short black_pawn_double_advance_row = 6;
const short king_init_col = 4;
const short king_castle_long_col = 2;
const short king_castle_short_col = 6;
const short rook_castle_long_col = 3;
const short rook_castle_short_col = 5;

const short empty_square = 0;

const short white_pawn = 11;
const short white_rook = 12;
const short white_knight = 13;
const short white_bishop = 14;
const short white_king = 15;
const short white_queen = 16;

const short black_pawn = 21;
const short black_rook = 22;
const short black_knight = 23;
const short black_bishop = 24;
const short black_king = 25;
const short black_queen = 26;

bool is_white(short piece) {
    return piece < 20;
};

short internal_to_printed(short in) {
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
    "moves: [", board_->move == 0 ? 0 : board_->trace[board_->move - 1].flags, board_->white_to_move,
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

/*
    ranges: valid pairs of distances for a piece to move on each axis
*/

const short knight_ranges[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
const short king_ranges[8][2] = {{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
const short white_pawn_capture_ranges[2][2] = {{1, 1}, {-1, 1}};
const short black_pawn_capture_ranges[2][2] = {{1, -1}, {-1, -1}};

#endif