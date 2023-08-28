#ifndef heuristics
#define heuristics

#include "data_structs.hpp"

const float pawn_simple_value = 1;
const float rook_simple_value = 5;
const float knight_simple_value = 3;
const float bishop_simple_value = 3;
const float queen_simple_value = 9;

float simple_material_heuristic(board * board_) {
    float result = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch (board_->piece_positions[i][j]) {
            case white_pawn:
                result += pawn_simple_value;
                break;
            case black_pawn:
                result -= pawn_simple_value;
                break;
            case white_rook:
                result += rook_simple_value;
                break;
            case black_rook:
                result -= rook_simple_value;
                break;    
            case white_knight:
                result += knight_simple_value;
                break;
            case black_knight:
                result -= knight_simple_value;
                break;    
            case white_bishop:
                result += bishop_simple_value;
                break;
            case black_bishop:
                result -= bishop_simple_value;
                break;    
            case white_queen:
                result += queen_simple_value;
                break;
            case black_queen:
                result -= queen_simple_value;
                break;       
            default:
                break;
            };
        };
    };
    return result;
};

#endif