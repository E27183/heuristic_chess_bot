#ifndef moving
#define moving

#include "data_structs.hpp"
#include "utils.hpp"
#include <cmath>

void init_board(board * unset) {
    unset->move = 0;
    unset->drawn = false;
    unset->white_to_move = true;
    short init[8][8] = {
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

void make_move(move * move_, board * board_, short promotion_target = 0) { // Does not check legality, please check move is legal before calling this to avoid undefined behaviour
    short from_piece = board_->piece_positions[move_->square_from[0]][move_->square_from[1]];
    short to_piece = board_->piece_positions[move_->square_to[0]][move_->square_to[1]];
    move_internal current_move = {
        square_from: {move_->square_from[0], move_->square_from[1]},
        square_to: {move_->square_to[0], move_->square_to[1]},
        taken_piece: 0,
        flags: 0
    };
    if (board_->move == 0) {
        current_move.flags = white_castle_long | white_castle_short | black_castle_long | black_castle_short;
    } else {
        current_move.flags = (white_castle_long | white_castle_short | black_castle_long | black_castle_short) & board_->trace[board_->move - 1].flags;
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
    if (from_piece == white_rook && move_->square_from[1] == white_king_castle_row) {
        if (move_->square_from[0] == short_castle_to_clean_col) {
            current_move.flags = current_move.flags ^ white_castle_short;
        } else if (move_->square_from[0] == long_castle_to_clean_col) {
            current_move.flags = current_move.flags ^ white_castle_long;
        };
    };
    if (from_piece == black_rook && move_->square_from[1] == black_king_castle_row) {
        if (move_->square_from[0] == short_castle_to_clean_col) {
            current_move.flags = current_move.flags ^ black_castle_short;
        } else if (move_->square_from[0] == long_castle_to_clean_col) {
            current_move.flags = current_move.flags ^ black_castle_long;
        };
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
    if (from_piece == white_pawn && move_->square_to[1] == white_promote_row) {
        current_move.taken_piece = promotion_event;
        board_->piece_positions[move_->square_from[0]][move_->square_from[1]] = empty_square;
        board_->piece_positions[move_->square_to[0]][move_->square_to[1]] = promotion_target;
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

bool is_in_bounds(short location[2]) {
    return location[0] >= 0 && location[0] < 8 && location[1] >= 0 && location[1] < 8;
};

void scan_for_king(board * board_, bool white, short return_buffer[2]) {
    if (white) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board_->piece_positions[i][j] == white_king) {
                    return_buffer[0] = i;
                    return_buffer[1] = j;
                    return;
                };
            };
        };
    } else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board_->piece_positions[i][j] == black_king) {
                    return_buffer[0] = i;
                    return_buffer[1] = j;
                    return;
                };
            };
        };
    };
};

bool is_checked_excluding_vector_travelstyle_pieces(board * board_, bool white, short king_position[2]) {
    short own_king = white ? white_king : black_king;
    short opp_king = white ? black_king : white_king;
    short opp_knight = white ? black_knight : white_knight;
    short opp_pawn = white ? black_pawn : white_pawn;
    for (int i = 0; i < 8; i++) { // Checked by knight?
        short target[2] = {static_cast<short>(king_position[0] + knight_ranges[i][0]), static_cast<short>(king_position[1] + knight_ranges[i][1])};
        if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_knight) {
            return true;
        };
    };
    for (int i = 0; i < 8; i++) { // Checked by king?
        short target[2] = {static_cast<short>(king_position[0] + king_ranges[i][0]), static_cast<short>(king_position[1] + king_ranges[i][1])};
        if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_king) {
            return true;
        };
    };
    if (white) { // Checked by pawn?
        for (int i = 0; i < 2; i++) {
            short target[2] = {static_cast<short>(king_position[0] + white_pawn_capture_ranges[i][0]), static_cast<short>(king_position[1] + white_pawn_capture_ranges[i][1])}; // Using white to search for black pawns is correct here, as they are perfectly assymetric and we are looking from the perspective of the attacked piece
            if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_pawn) {
              return true;
            };
        };
    } else {
        for (int i = 0; i < 2; i++) {
            short target[2] = {static_cast<short>(king_position[0] + black_pawn_capture_ranges[i][0]), static_cast<short>(king_position[1] + black_pawn_capture_ranges[i][1])};
            if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_pawn) {
              return true;
            };
        };
    };
    return false;
};

bool is_checked(board * board_, bool white) {
    short own_king = white ? white_king : black_king;
    short opp_rook = white ? black_rook : white_rook;
    short opp_bishop = white ? black_bishop : white_bishop;
    short opp_queen = white ? black_queen : white_queen;
    short king_position[2];
    scan_for_king(board_, white, king_position);
    if (is_checked_excluding_vector_travelstyle_pieces(board_, white, king_position)) {
        return true;
    };
    for (int i = king_position[0] + 1; i < 8; i++) { // Checked from right?
        short piece = board_->piece_positions[i][king_position[1]];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = king_position[0] - 1; i >= 0; i--) { // Checked from left?
        short piece = board_->piece_positions[i][king_position[1]];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = king_position[1] + 1; i < 8; i++) { // Checked from up?
        short piece = board_->piece_positions[king_position[0]][i];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = king_position[1] - 1; i >= 0; i--) { // Checked from down?
        short piece = board_->piece_positions[king_position[0]][i];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = 1; true; i++) { // Checked from top-right?
        short target[2] = {static_cast<short>(king_position[0] + i), static_cast<short>(king_position[1] + i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = 1; true; i++) { // Checked from top-left?
        short target[2] = {static_cast<short>(king_position[0] + i), static_cast<short>(king_position[1] - i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (short i = 1; true; i++) { // Checked from bottom-right?
        short target[2] = {static_cast<short>(king_position[0] - i), static_cast<short>(king_position[1] + i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    for (int i = 1; true; i++) { // Checked from bottom-left?
        short target[2] = {static_cast<short>(king_position[0] - i), static_cast<short>(king_position[1] - i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                return true;
            } else {
                break;
            };
        };
    };
    return false;
};

void has_checks_excluding_vector_travelstyle_pieces(board * board_, bool white, short king_position[2], check_and_pin_feedback * feedback) { 
    short own_king = white ? white_king : black_king;
    short opp_king = white ? black_king : white_king;
    short opp_knight = white ? black_knight : white_knight;
    short opp_pawn = white ? black_pawn : white_pawn;
    for (int i = 0; i < 8; i++) { // Checked by knight?
        short target[2] = {static_cast<short>(king_position[0] + knight_ranges[i][0]), static_cast<short>(king_position[1] + knight_ranges[i][1])};
        if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_knight) {
            if (feedback->checks == 0) {
                feedback->checks = 1;
                feedback->valid_squares.insert((target[0] << 6) + target[1]);
            } else {
                feedback->checks = 2;
                return;
            };
        };
    };
    for (int i = 0; i < 8; i++) { // Checked by king?
        short target[2] = {static_cast<short>(king_position[0] + king_ranges[i][0]), static_cast<short>(king_position[1] + king_ranges[i][1])};
        if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_king) {
            if (feedback->checks == 0) {
                feedback->checks = 1;
                feedback->valid_squares.insert((target[0] << 6) + target[1]);
            } else {
                feedback->checks = 2;
                return;
            };
        };
    };
    if (white) { // Checked by pawn?
        for (int i = 0; i < 2; i++) {
            short target[2] = {static_cast<short>(king_position[0] + white_pawn_capture_ranges[i][0]), static_cast<short>(king_position[1] + white_pawn_capture_ranges[i][1])}; // Using white to search for black pawns is correct here, as they are perfectly assymetric and we are looking from the perspective of the attacked piece
            if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_pawn) {
                if (feedback->checks == 0) {
                    feedback->checks = 1;
                    feedback->valid_squares.insert((target[0] << 6) + target[1]);
                } else {
                    feedback->checks = 2;
                    return;
                };
            };
        };
    } else {
        for (int i = 0; i < 2; i++) {
            short target[2] = {static_cast<short>(king_position[0] + black_pawn_capture_ranges[i][0]), static_cast<short>(king_position[1] + black_pawn_capture_ranges[i][1])};
            if (is_in_bounds(target) && board_->piece_positions[target[0]][target[1]] == opp_pawn) {
                if (feedback->checks == 0) {
                    feedback->checks = 1;
                    feedback->valid_squares.insert((target[0] << 6) + target[1]);
                } else {
                    feedback->checks = 2;
                    return;
                };
            };
        };
    };
};

void is_checked_and_identify_pins_and_freedoms(board * board_, bool white, check_and_pin_feedback * feedback) { // As well as determining if a direct check exists, determine where can be moved to stop the direct check and if any pieces are pinned by threats
    short own_king = white ? white_king : black_king;
    short opp_rook = white ? black_rook : white_rook;
    short opp_bishop = white ? black_bishop : white_bishop;
    short opp_queen = white ? black_queen : white_queen;
    short king_position[2];
    scan_for_king(board_, white, king_position);
    feedback->checks = 0;
    feedback->pinned_pieces_count = 0;
    feedback->valid_squares.clear();
    has_checks_excluding_vector_travelstyle_pieces(board_, white, king_position, feedback);
    if (feedback->checks > 1) {
        return;
    };
    short pinned_piece = empty_square;
    for (int i = king_position[0] + 1; i < 8; i++) { // Checked from right?
        short piece = board_->piece_positions[i][king_position[1]];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = i;
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = king_position[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = left_right;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = king_position[0] + 1; j <= i; j++) {
                        feedback->valid_squares.insert((static_cast<short>(j) << 6) + king_position[1]);                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = king_position[0] - 1; i >= 0; i--) { // Checked from left?
        short piece = board_->piece_positions[i][king_position[1]];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = i;
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = king_position[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = left_right;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = king_position[0] - 1; j >= i; j--) {
                        feedback->valid_squares.insert((static_cast<short>(j) << 6) + king_position[1]);                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = king_position[1] + 1; i < 8; i++) { // Checked from up?
        short piece = board_->piece_positions[king_position[0]][i];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = king_position[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = i;
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = up_down;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = king_position[1] + 1; j <= i; j++) {
                        feedback->valid_squares.insert((king_position[0] << 6) + static_cast<short>(j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = king_position[1] - 1; i >= 0; i--) { // Checked from down?
        short piece = board_->piece_positions[king_position[0]][i];
        if (piece != empty_square) {
            if (piece == opp_rook || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = king_position[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = i;
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = up_down;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = king_position[1] - 1; j >= i; j--) {
                        feedback->valid_squares.insert((king_position[0] << 6) + static_cast<short>(j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = 1; true; i++) { // Checked from top-right?
        short target[2] = {static_cast<short>(king_position[0] + i), static_cast<short>(king_position[1] + i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = target[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = target[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = ascending;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = 1; j <= i; j++) {
                        feedback->valid_squares.insert((static_cast<short>(king_position[0] + j) << 6) + static_cast<short>(king_position[1] + j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = 1; true; i++) { // Checked from top-left?
        short target[2] = {static_cast<short>(king_position[0] + i), static_cast<short>(king_position[1] - i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = target[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = target[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = descending;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = 1; j <= i; j++) {
                        feedback->valid_squares.insert((static_cast<short>(king_position[0] + j) << 6) + static_cast<short>(king_position[1] - j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = 1; true; i++) { // Checked from bottom-right?
        short target[2] = {static_cast<short>(king_position[0] - i), static_cast<short>(king_position[1] + i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = target[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = target[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = descending;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = 1; j <= i; j++) {
                        feedback->valid_squares.insert((static_cast<short>(king_position[0] - j) << 6) + static_cast<short>(king_position[1] + j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
    };
    pinned_piece = empty_square;
    for (int i = 1; true; i++) { // Checked from bottom-left?
        short target[2] = {static_cast<short>(king_position[0] - i), static_cast<short>(king_position[1] - i)};
        if (!is_in_bounds(target)) {
            break;
        };
        short piece = board_->piece_positions[target[0]][target[1]];
        if (piece != empty_square) {
            if (piece == opp_bishop || piece == opp_queen) {
                if (pinned_piece != empty_square) {
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[0] = target[0];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].location[1] = target[1];
                    feedback->pinned_pieces[feedback->pinned_pieces_count].direction = ascending;
                } else if (feedback->checks > 0) {
                    feedback->checks = 2;
                    return;
                } else {
                    feedback->checks++;
                    for (int j = 1; j <= i; j++) {
                        feedback->valid_squares.insert((static_cast<short>(king_position[0] - j) << 6) + static_cast<short>(king_position[1] - j));                        
                    };
                };
            } else if (pinned_piece == empty_square && is_white(piece) == white) {
                pinned_piece = piece;
            } else {
                break;
            };
        };
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
        const short to_piece = board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]];
        if (move_to_undo.taken_piece == en_passant_event) {
            if (to_piece == white_pawn) {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1] - 1] = black_pawn;
            } else {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1] + 1] = white_pawn;
            };
            board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]] = empty_square;
        } else {
            if (move_to_undo.taken_piece != promotion_event) {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]] = move_to_undo.taken_piece;
            } else {
                board_->piece_positions[move_to_undo.square_to[0]][move_to_undo.square_to[1]] = empty_square;
            };
        }
        if (move_to_undo.taken_piece != promotion_event) {
            board_->piece_positions[move_to_undo.square_from[0]][move_to_undo.square_from[1]] = to_piece;
        } else {
            board_->piece_positions[move_to_undo.square_from[0]][move_to_undo.square_from[1]] = is_white(to_piece) ? white_pawn : black_pawn;
        };
    };
};

bool is_saving(move * move_, check_and_pin_feedback * feedback) { // Returns true unless a move fails to block a check or creates a new check. Does not account for double checks and should not be passed this situation
    for (int i = 0; i < feedback->pinned_pieces_count; i++) {
        if (feedback->pinned_pieces[i].location[0] == move_->square_from[0] && feedback->pinned_pieces[i].location[1] == move_->square_from[1]) {
            if (feedback->pinned_pieces[i].direction == up_down && move_->square_from[0] != move_->square_to[0]) {
                return false;
            };
            if (feedback->pinned_pieces[i].direction == left_right && move_->square_from[1] != move_->square_to[1]) {
                return false;
            };
            if (feedback->pinned_pieces[i].direction == ascending && move_->square_from[0] - move_->square_to[0] != move_->square_from[1] - move_->square_to[1]) {
                return false;
            };
            if (feedback->pinned_pieces[i].direction == descending && move_->square_from[0] - move_->square_to[0] != move_->square_to[1] - move_->square_from[1]) {
                return false;
            };
            break;
        };
    };
    return feedback->checks == 0 || feedback->valid_squares.count(static_cast<short>(move_->square_to[0] << 6) + move_->square_to[1]);
};

bool valid_move(move * move_, board * board_, check_and_pin_feedback * feedback, bool confirmed_clear_path = false) {
    if (!is_in_bounds(move_->square_from) || !is_in_bounds(move_->square_to)) {
        return false;
    };
    if (move_->square_from[0] == move_->square_to[0] && move_->square_from[1] == move_->square_to[1]) {
        return false;
    };
    short piece_to_move = board_->piece_positions[move_->square_from[0]][move_->square_from[1]];
    short piece_to_land = board_->piece_positions[move_->square_to[0]][move_->square_to[1]];
    if (piece_to_move == empty_square || is_white(piece_to_move) != board_->white_to_move) {
        return false;
    };
    if (piece_to_land != empty_square && is_white(piece_to_land) == is_white(piece_to_move)) {
        return false;
    };
    short own_king = board_->white_to_move ? white_king : black_king;
    short own_rook = board_->white_to_move ? white_rook : black_rook;
    short own_knight = board_->white_to_move ? white_knight : black_knight;
    short own_bishop = board_->white_to_move ? white_bishop : black_bishop;
    short own_queen = board_->white_to_move ? white_queen : black_queen;
    short own_pawn = board_->white_to_move ? white_pawn : black_pawn;
    if (piece_to_move == own_king) {
        if (abs(move_->square_from[0] - move_->square_to[0]) > 1 || abs(move_->square_from[1] - move_->square_to[1]) > 1) {
            if (is_checked(board_, board_->white_to_move)) {
                return false;
            };
            if (board_->white_to_move && move_->square_from[1] == white_king_castle_row && move_->square_from[0] == king_init_col && move_->square_to[1] == white_king_castle_row) {
                if (board_->trace[board_->move - 1].flags & white_castle_short && move_->square_to[0] == king_castle_short_col) {
                    for (const short &col : king_castle_short_cols_to_be_empty_or_checked) {
                        if (board_->piece_positions[col][white_king_castle_row] != empty_square) {
                            return false;
                        } else {
                            board_->piece_positions[col][white_king_castle_row] = white_king;
                            board_->piece_positions[move_->square_from[0]][white_king_castle_row] = empty_square;
                            bool legal = !is_checked(board_, board_->white_to_move);
                            board_->piece_positions[col][white_king_castle_row] = empty_square;
                            board_->piece_positions[move_->square_from[0]][white_king_castle_row] = white_king;
                            if (!legal) {
                                return false;
                            };
                        };
                    };
                    return true;
                } else if (board_->trace[board_->move - 1].flags & white_castle_long && move_->square_to[0] == king_castle_long_col) {
                    for (const short &col : king_castle_long_cols_to_be_empty) {
                        if (board_->piece_positions[col][white_king_castle_row] != empty_square) {
                            return false;
                        };
                    };
                    for (const short &col : king_castle_long_cols_to_be_not_checked) {
                        board_->piece_positions[col][white_king_castle_row] = white_king;
                        board_->piece_positions[move_->square_from[0]][white_king_castle_row] = empty_square;
                        bool legal = !is_checked(board_, board_->white_to_move);
                        board_->piece_positions[col][white_king_castle_row] = empty_square;
                        board_->piece_positions[move_->square_from[0]][white_king_castle_row] = white_king;
                        if (!legal) {
                            return false;
                        };
                    };
                } else {
                    return false;
                };
            } else if (!board_->white_to_move && move_->square_from[1] == black_king_castle_row && move_->square_from[0] == king_init_col && move_->square_to[1] == black_king_castle_row) {
                if (board_->trace[board_->move - 1].flags & black_castle_short && move_->square_to[0] == king_castle_short_col) {
                    for (const short &col : king_castle_short_cols_to_be_empty_or_checked) {
                        if (board_->piece_positions[col][black_king_castle_row] != empty_square) {
                            return false;
                        } else {
                            board_->piece_positions[col][black_king_castle_row] = black_king;
                            board_->piece_positions[move_->square_from[0]][black_king_castle_row] = empty_square;
                            bool legal = !is_checked(board_, board_->white_to_move);
                            board_->piece_positions[col][black_king_castle_row] = empty_square;
                            board_->piece_positions[move_->square_from[0]][black_king_castle_row] = black_king;
                            if (!legal) {
                                return false;
                            };
                        };
                    };
                    return true;
                } else if (board_->trace[board_->move - 1].flags & black_castle_long && move_->square_to[0] == king_castle_long_col) {
                    for (const short &col : king_castle_long_cols_to_be_empty) {
                        if (board_->piece_positions[col][black_king_castle_row] != empty_square) {
                            return false;
                        };
                    };
                    for (const short &col : king_castle_long_cols_to_be_not_checked) {
                        board_->piece_positions[col][black_king_castle_row] = black_king;
                        board_->piece_positions[move_->square_from[0]][black_king_castle_row] = empty_square;
                        bool legal = !is_checked(board_, board_->white_to_move);
                        board_->piece_positions[col][black_king_castle_row] = empty_square;
                        board_->piece_positions[move_->square_from[0]][black_king_castle_row] = black_king;
                        if (!legal) {
                            return false;
                        };
                    };
                } else {
                    return false;
                };
            } else {
                return false;
            };
        } else {
            short piece_in_memory = board_->piece_positions[move_->square_to[0]][move_->square_to[1]];
            board_->piece_positions[move_->square_to[0]][move_->square_to[1]] = own_king;
            board_->piece_positions[move_->square_from[0]][move_->square_from[1]] = empty_square;
            bool legal = !is_checked(board_, board_->white_to_move);
            board_->piece_positions[move_->square_to[0]][move_->square_to[1]] = piece_in_memory;
            board_->piece_positions[move_->square_from[0]][move_->square_from[1]] = own_king;
            return legal;
        };
    } else {
        if (feedback->checks > 1) {
            return false;
        };
        if (piece_to_move == own_rook) {
            if (move_->square_from[0] != move_->square_to[0] && move_->square_from[1] != move_->square_to[1]) {
                return false;
            } else if (move_->square_to[0] > move_->square_from[0]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[0] + 1; i < move_->square_to[0]; i++) {
                        if (board_->piece_positions[i][move_->square_from[1]] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] < move_->square_from[0]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[0] - 1; i > move_->square_to[0]; i--) {
                        if (board_->piece_positions[i][move_->square_from[1]] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[1] > move_->square_from[1]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[1] + 1; i < move_->square_to[1]; i++) {
                        if (board_->piece_positions[move_->square_from[0]][i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[1] - 1; i > move_->square_to[1]; i--) {
                        if (board_->piece_positions[move_->square_from[0]][i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            };
        } else if (piece_to_move == own_knight) {
            if (abs(move_->square_from[0] - move_->square_to[0]) + abs(move_->square_from[1] - move_->square_to[1]) != 3) {
                return false;
            } else {
                return is_saving(move_, feedback);
            };
        } else if (piece_to_move == own_bishop) {
            if (abs(move_->square_from[0] - move_->square_to[0]) != abs(move_->square_from[1] - move_->square_to[1])) {
                return false;
            } else if (move_->square_to[0] > move_->square_from[0] && move_->square_to[1] > move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = 1; i < move_->square_to[0] - move_->square_from[0]; i++) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] + i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] > move_->square_from[0] && move_->square_to[1] < move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = 1; i < move_->square_to[0] - move_->square_from[0]; i++) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] - i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] < move_->square_from[0] && move_->square_to[1] > move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = -1; i > move_->square_to[0] - move_->square_from[0]; i--) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] - i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else {
                if (!confirmed_clear_path) {
                    for (int i = -1; i > move_->square_to[0] - move_->square_from[0]; i--) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] + i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            };
        } else if (piece_to_move == own_queen) {
            if ((move_->square_from[0] != move_->square_to[0] && move_->square_from[1] != move_->square_to[1]) && (abs(move_->square_from[0] - move_->square_to[0]) != abs(move_->square_from[1] - move_->square_to[1]))) {
                return false;
            } else if (move_->square_to[0] > move_->square_from[0] && move_->square_from[1] == move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[0] + 1; i < move_->square_to[0]; i++) {
                        if (board_->piece_positions[i][move_->square_from[1]] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] < move_->square_from[0] && move_->square_from[1] == move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[0] - 1; i > move_->square_to[0]; i--) {
                        if (board_->piece_positions[i][move_->square_from[1]] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[1] > move_->square_from[1] && move_->square_from[1] == move_->square_to[0]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[1] + 1; i < move_->square_to[1]; i++) {
                        if (board_->piece_positions[move_->square_from[0]][i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[1] < move_->square_from[1] && move_->square_from[1] == move_->square_to[0]) {
                if (!confirmed_clear_path) {
                    for (int i = move_->square_from[1] - 1; i > move_->square_to[1]; i--) {
                        if (board_->piece_positions[move_->square_from[0]][i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] > move_->square_from[0] && move_->square_to[1] > move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = 1; i < move_->square_to[0] - move_->square_from[0]; i++) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] + i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] > move_->square_from[0] && move_->square_to[1] < move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = 1; i < move_->square_to[0] - move_->square_from[0]; i++) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] - i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else if (move_->square_to[0] < move_->square_from[0] && move_->square_to[1] > move_->square_to[1]) {
                if (!confirmed_clear_path) {
                    for (int i = -1; i > move_->square_to[0] - move_->square_from[0]; i--) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] - i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            } else {
                if (!confirmed_clear_path) {
                    for (int i = -1; i > move_->square_to[0] - move_->square_from[0]; i--) {
                        if (board_->piece_positions[move_->square_from[0] + i][move_->square_from[1] + i] != empty_square) {
                            return false;
                        };
                    };
                };
                return is_saving(move_, feedback);
            };
        } else { // last case is pawn
            if (move_->square_to[0] == move_->square_from[0]) {
                if (piece_to_land != empty_square) {
                    return false;
                };
                if (piece_to_move == white_pawn) {
                    if (move_->square_to[1] - move_->square_from[1] == 1) {
                        return is_saving(move_, feedback);
                    } else if (move_->square_to[1] - move_->square_from[1] == 2 && move_->square_from[1] == white_pawn_double_advance_row && board_->piece_positions[move_->square_from[0]][move_->square_from[1] + 1] == empty_square) {
                        return is_saving(move_, feedback);
                    } else {
                        return false;
                    };
                } else {
                    if (move_->square_from[1] - move_->square_to[1] == 1) {
                        return is_saving(move_, feedback);
                    } else if (move_->square_from[1] - move_->square_to[1] == 2 && move_->square_from[1] == black_pawn_double_advance_row && board_->piece_positions[move_->square_from[0]][move_->square_from[1] - 1] == empty_square) {
                        return is_saving(move_, feedback);
                    } else {
                        return false;
                    };
                };
            } else {
                if (abs(move_->square_to[0] - move_->square_from[0]) != 1) {
                    return false;
                };
                if (piece_to_move == white_pawn) {
                    if (move_->square_to[1] - move_->square_from[1] != 1) {
                        return false;
                    };
                    if (piece_to_land != empty_square) {
                        return is_saving(move_, feedback);
                    } else {
                        if (board_->move == 0) {
                            return false;
                        };
                        move_internal last_move = board_->trace[board_->move - 1];
                        if (!(last_move.flags & can_en_passant) || 
                        last_move.square_to[0] != move_->square_to[0] || 
                        last_move.square_to[1] != move_->square_to[1] - 1) {
                            return false;
                        } else if (is_saving(move_, feedback)) {
                            return true;
                        } else {
                            if (feedback->valid_squares.count((move_->square_to[0] << 6) + move_->square_to[1] - 1) == 0) {
                                return false;
                            } else {
                                for (int  i = 0; i < feedback->pinned_pieces_count; i++) {
                                    if (feedback->pinned_pieces[i].location[0] == move_->square_from[0] && feedback->pinned_pieces[i].location[0] == move_->square_from[0]) {
                                        if (feedback->pinned_pieces[i].direction == ascending) {
                                            return move_->square_to[1] - move_->square_from[1] == 1;
                                        } else if (feedback->pinned_pieces[i].direction == descending) {
                                            return move_->square_from[1] - move_->square_to[1] == 1;
                                        } else {
                                            return false;
                                        };
                                    };
                                };
                                return true;
                            };
                        };
                    };
                } else {
                    if (move_->square_from[1] - move_->square_to[1] != 1) {
                        return false;
                    };
                    if (piece_to_land != empty_square) {
                        return is_saving(move_, feedback);
                    } else {
                        if (board_->move == 0) {
                            return false;
                        };
                        move_internal last_move = board_->trace[board_->move - 1];
                        if (!(last_move.flags & can_en_passant) || 
                        last_move.square_to[0] != move_->square_to[0] || 
                        last_move.square_to[1] != move_->square_to[1] + 1) {
                            return false;
                        } else if (is_saving(move_, feedback)) {
                            return true;
                        } else {
                            if (feedback->valid_squares.count((move_->square_to[0] << 6) + move_->square_to[1] + 1) == 0) {
                                return false;
                            } else {
                                for (int  i = 0; i < feedback->pinned_pieces_count; i++) {
                                    if (feedback->pinned_pieces[i].location[0] == move_->square_from[0] && feedback->pinned_pieces[i].location[0] == move_->square_from[0]) {
                                        if (feedback->pinned_pieces[i].direction == ascending) {
                                            return move_->square_from[1] - move_->square_to[1] == 1;
                                        } else if (feedback->pinned_pieces[i].direction == descending) {
                                            return move_->square_to[1] - move_->square_from[1] == 1;
                                        } else {
                                            return false;
                                        };
                                    };
                                };
                                return true;
                            };
                        };
                    };
                };
            };
        };
    };
    return true;
};

#endif