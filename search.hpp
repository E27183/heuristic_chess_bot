#ifndef search
#define search

#include <thread>
#include <mutex>
#include <vector>
#include "data_structs.hpp"
#include "moving.hpp"
#include "utils.hpp"
#include "heuristics.hpp"

const unsigned int numCores = min(std::thread::hardware_concurrency(), 20);

void assign_move_if_possible(board * board_, move * candidate, short to_row, short to_col, move buffer[256], short * moves_len, check_and_pin_feedback * feedback) {
    candidate->square_to[0] = to_row;
    candidate->square_to[1] = to_col;
    if (valid_move(candidate, board_, feedback, true)) {
        buffer[*moves_len].square_from[0] = candidate->square_from[0];
        buffer[*moves_len].square_from[1] = candidate->square_from[1];
        buffer[*moves_len].square_to[0] = candidate->square_to[0];
        buffer[*moves_len].square_to[1] = candidate->square_to[1];
        (*moves_len)++;
    };
};

void populate_buffer_with_legal_moves(board * board_, move buffer[256], short * moves_len) {
    *moves_len = 0;
    check_and_pin_feedback feedback;
    is_checked_and_identify_pins_and_freedoms(board_, board_->white_to_move, &feedback);
    short own_king = board_->white_to_move ? white_king : black_king;
    short own_queen = board_->white_to_move ? white_queen : black_queen;
    short own_bishop = board_->white_to_move ? white_bishop : black_bishop;
    short own_knight = board_->white_to_move ? white_knight : black_knight;
    short own_rook = board_->white_to_move ? white_rook : black_rook;
    short own_pawn = board_->white_to_move ? white_pawn : black_pawn;
    move candidate;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            candidate.square_from[0] = i;
            candidate.square_from[1] = j;
            short piece = board_->piece_positions[i][j];
            if (piece != empty_square && is_white(piece) == board_->white_to_move) {
                if (piece == own_king) {
                    for (int k = 0; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, i + king_ranges[k][0], j + king_ranges[k][1], buffer, moves_len, &feedback);
                    };
                } else if (piece == own_knight) {
                    for (int k = 0; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, i + knight_ranges[k][0], j + knight_ranges[k][1], buffer, moves_len, &feedback);
                    };
                } else if (piece == own_rook) {
                    for (int k = i + 1; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[k][j] != empty_square) {
                            break;
                        };
                    };
                    for (int k = i - 1; k >= 0; k--) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[k][j] != empty_square) {
                            break;
                        };
                    };
                    for (int k = j + 1; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = j - 1; k >= 0; k--) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                } else if (piece == own_bishop) {
                    for (int k = 0; true; k++) {
                        if (i + k >= 8 || j + k >= 8) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i + k, j + k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i + k >= 8 || j - k < 0) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i + k, j - k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i - k < 0 || j - k < 0) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i - k, j - k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i + k < 0 || j + k >= 8) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i - k, j + k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                } else if (piece == own_queen) {
                    for (int k = i + 1; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[k][j] != empty_square) {
                            break;
                        };
                    };
                    for (int k = i - 1; k >= 0; k--) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[k][j] != empty_square) {
                            break;
                        };
                    };
                    for (int k = j + 1; k < 8; k++) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = j - 1; k >= 0; k--) {
                        assign_move_if_possible(board_, &candidate, k, j, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i + k >= 8 || j + k >= 8) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i + k, j + k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i + k >= 8 || j - k < 0) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i + k, j - k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i - k < 0 || j - k < 0) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i - k, j - k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                    for (int k = 0; true; k++) {
                        if (i + k < 0 || j + k >= 8) {
                            break;
                        };
                        assign_move_if_possible(board_, &candidate, i - k, j + k, buffer, moves_len, &feedback);
                        if (board_->piece_positions[i][k] != empty_square) {
                            break;
                        };
                    };
                } else {
                    if (board_->white_to_move) {
                        assign_move_if_possible(board_, &candidate, i, j + 1, buffer, moves_len, &feedback);
                        if (i < 7 && j < 7 && (board_->piece_positions[i + 1][j + 1] != empty_square || j + 1 == white_en_passant_row)) {
                            assign_move_if_possible(board_, &candidate, i + 1, j + 1, buffer, moves_len, &feedback);
                        };
                        if (i > 0 && j < 7 && (board_->piece_positions[i - 1][j + 1] != empty_square || j + 1 == black_en_passant_row)) {
                            assign_move_if_possible(board_, &candidate, i - 1, j + 1, buffer, moves_len, &feedback);
                        };
                        if (j == white_pawn_double_advance_row) {
                            assign_move_if_possible(board_, &candidate, i, j + 2, buffer, moves_len, &feedback);
                        };
                    } else {
                        assign_move_if_possible(board_, &candidate, i, j - 1, buffer, moves_len, &feedback);
                        if (i < 7 && j > 0 && (board_->piece_positions[i + 1][j - 1] != empty_square || j + 1 == white_en_passant_row)) {
                            assign_move_if_possible(board_, &candidate, i + 1, j - 1, buffer, moves_len, &feedback);
                        };
                        if (i > 0 && j > 0 && (board_->piece_positions[i - 1][j - 1] != empty_square || j + 1 == black_en_passant_row)) {
                            assign_move_if_possible(board_, &candidate, i - 1, j - 1, buffer, moves_len, &feedback);
                        };
                        if (j == black_pawn_double_advance_row) {
                            assign_move_if_possible(board_, &candidate, i, j - 2, buffer, moves_len, &feedback);
                        };
                    };
                };
            };
        };
    };
};

void worker_recursive_call(board * board_, short depth, float * eval) {};

void worker_thread(board * board_, std::mutex * access_lock, short moves_len, short * current_move_index, short depth, move buffer_orig[256], short * best_move_index, short * best_move_eval, std::mutex * best_move_lock) {
    board my_local_board;
    deep_copy(board_, &my_local_board);
    move buffer[256];
    short moves_found;
    while (true) {
        short index_this_cycle;
        access_lock->lock();
        if (*current_move_index >= moves_len) {
            access_lock->unlock();
            return;
        } else {
            index_this_cycle = *current_move_index;
            (*current_move_index)++;
            access_lock->unlock();
        };
        make_move(&buffer_orig[index_this_cycle], &my_local_board);
        float eval_of_this_move = board_->white_to_move ? -MAXFLOAT : MAXFLOAT;
        moves_found = 0;
        populate_buffer_with_legal_moves(&my_local_board, buffer, &moves_found);
        if (moves_found == 0) {
            if (is_checked(&my_local_board, my_local_board.white_to_move)) {
                eval_of_this_move = my_local_board.white_to_move ? -MAXFLOAT : MAXFLOAT;
            } else {
                eval_of_this_move = 0;
            };
        } else {
            for (int i = 0; i < moves_found; i++) {
                if ((buffer[i].square_to[1] == white_promote_row && board_->piece_positions[buffer[i].square_from[0]][buffer[i].square_from[1]] == white_pawn) || 
                (buffer[i].square_to[1] == black_promote_row && board_->piece_positions[buffer[i].square_from[0]][buffer[i].square_from[1]] == black_pawn)) {
                    if (my_local_board.white_to_move) {
                        for (const short &piece : promotable_white_pieces) {
                            make_move(&buffer[i], &my_local_board, piece);
                            float eval;
                            if (depth == 0) {
                                eval = applied_heuristic(&my_local_board);
                            } else {
                                worker_recursive_call(&my_local_board, depth - 1, &eval);
                            };
                            if ((eval > eval_of_this_move && !board_->white_to_move) || (eval < eval_of_this_move && board_->white_to_move)) { // Is reversed as board being white to move on core board means most recent move was made by black
                                eval_of_this_move = eval;
                            };
                            reset_recent_move(&my_local_board);
                        };
                    } else {
                        for (const short &piece : promotable_black_pieces) {
                            make_move(&buffer[i], &my_local_board, piece);
                            float eval;
                            if (depth == 0) {
                                eval = applied_heuristic(&my_local_board);
                            } else {
                                worker_recursive_call(&my_local_board, depth - 1, &eval);
                            };
                            if ((eval > eval_of_this_move && !board_->white_to_move) || (eval < eval_of_this_move && board_->white_to_move)) { // Is reversed as board being white to move on core board means most recent move was made by black
                                eval_of_this_move = eval;
                            };
                            reset_recent_move(&my_local_board);
                        };
                    };
                } else {
                    make_move(&buffer[i], &my_local_board);
                    float eval;
                    if (depth == 0) {
                        eval = applied_heuristic(&my_local_board);
                    } else {
                        worker_recursive_call(&my_local_board, depth - 1, &eval);
                    };
                    if ((eval > eval_of_this_move && !board_->white_to_move) || (eval < eval_of_this_move && board_->white_to_move)) { // Is reversed as board being white to move on core board means most recent move was made by black
                        eval_of_this_move = eval;
                    };
                    reset_recent_move(&my_local_board);
                };
            };
        };
        reset_recent_move(&my_local_board);
        best_move_lock->lock();
        if ((board_->white_to_move && eval_of_this_move > *best_move_eval) || (!board_->white_to_move && eval_of_this_move < *best_move_eval)) {
            *best_move_eval = eval_of_this_move;
            *best_move_index = index_this_cycle;
        };
        best_move_lock->unlock();
    };
};

void search_for_best_move(board * board_, move * output_move, short * output_promotion_decision, short depth, bool * stale_or_checkmated) {
    move buffer[256];
    short moves_len = 0;
    short current_move_index = 0;
    short best_move_index;
    short best_move_eval = board_->white_to_move ? -MAXFLOAT : MAXFLOAT;
    std::mutex best_move_lock;
    std::mutex access_lock;
    std::vector<std::thread> workers;
    populate_buffer_with_legal_moves(board_, buffer, &moves_len);
    if (moves_len == 0) {
        *stale_or_checkmated = true;
        return;
    };
    for (int i = 0; i < numCores; i++) {
        workers.push_back(std::thread(worker_thread, board_, &access_lock, moves_len, &current_move_index, depth, buffer, &best_move_index, &best_move_eval, &best_move_lock));
    };
    for (std::thread &thread : workers) {
        thread.join();
    };
    std::cout << "is fin";
};

#endif