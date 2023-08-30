#include "data_structs.hpp"
#include "moving.hpp"
#include "search.hpp"
#include "utils.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Button.H>

Fl_Button *boxes[8][8];
int indices[8][8];
move move_;
bool first_move = true;
board board_;
check_and_pin_feedback user_check_pin_feedback;

struct {
    Fl_Image * white_pawn;
    Fl_Image * black_pawn;
    Fl_Image * white_rook;
    Fl_Image * black_rook;
    Fl_Image * white_knight;
    Fl_Image * black_knight;
    Fl_Image * white_bishop;
    Fl_Image * black_bishop;
    Fl_Image * white_queen;
    Fl_Image * black_queen;
    Fl_Image * white_king;
    Fl_Image * black_king;
} images;

void print_pieces_to_screen() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board_.piece_positions[i][j] == white_pawn) {
                boxes[i][j]->image(images.white_pawn);
            } else if (board_.piece_positions[i][j] == black_pawn) {
                boxes[i][j]->image(images.black_pawn);
            } else if (board_.piece_positions[i][j] == white_rook) {
                boxes[i][j]->image(images.white_rook);
            } else if (board_.piece_positions[i][j] == black_rook) {
                boxes[i][j]->image(images.black_rook);
            } else if (board_.piece_positions[i][j] == black_knight) {
                boxes[i][j]->image(images.black_knight);
            } else if (board_.piece_positions[i][j] == white_knight) {
                boxes[i][j]->image(images.white_knight);
            } else if (board_.piece_positions[i][j] == white_bishop) {
                boxes[i][j]->image(images.white_bishop);
            } else if (board_.piece_positions[i][j] == black_bishop) {
                boxes[i][j]->image(images.black_bishop);
            } else if (board_.piece_positions[i][j] == white_queen) {
                boxes[i][j]->image(images.white_queen);
            } else if (board_.piece_positions[i][j] == black_queen) {
                boxes[i][j]->image(images.black_queen);
            } else if (board_.piece_positions[i][j] == white_king) {
                boxes[i][j]->image(images.white_king);
            } else if (board_.piece_positions[i][j] == black_king) {
                boxes[i][j]->image(images.black_king);
            } else {
                boxes[i][j]->image(NULL);
            };
            boxes[i][j]->redraw();
        };
    };
};

void callbackfunc(Fl_Widget * widget, void * data) {
    int index = *(int*)data;
    int i = index >> 4;
    int j = index % 8;
    if (first_move) {
        first_move = false;
        move_.square_from[0] = i;
        move_.square_from[1] = j;
        widget->color(fl_rgb_color(255, 0, 0));
    } else {
        first_move = true;
        move_.square_to[0] = i;
        move_.square_to[1] = j;
        is_checked_and_identify_pins_and_freedoms(&board_, board_.white_to_move, &user_check_pin_feedback);
        if (valid_move(&move_, &board_, &user_check_pin_feedback)) {
            make_move(&move_, &board_);
            print_pieces_to_screen();
        };
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((i + j) % 2 == 0) {
                    boxes[i][j]->color(fl_rgb_color(100, 100, 100));
                    boxes[i][j]->redraw();
                } else {
                    boxes[i][j]->color(fl_rgb_color(0, 0, 200));
                    boxes[i][j]->redraw();
                };
            };
        };
    };
};

int main(int argc, char *argv[]) {
    int x;
    int y;
    int w;
    int h;
    Fl::screen_work_area(x, y, w, h, 0);
    Fl_Window *window = new Fl_Window(w, h, "Chess");
    int effectve_h = h - 100;
    int s = int_min(w, effectve_h);
    Fl_PNG_Image *wp = new Fl_PNG_Image("images/white_pawn.png");
    images.white_pawn = wp->copy(s / 9, s / 9);
    Fl_PNG_Image *bp = new Fl_PNG_Image("images/black_pawn.png");
    images.black_pawn = bp->copy(s / 9, s / 9);
    Fl_PNG_Image *wr = new Fl_PNG_Image("images/white_rook.png");
    images.white_rook = wr->copy(s / 9, s / 9);
    Fl_PNG_Image *br= new Fl_PNG_Image("images/black_rook.png");
    images.black_rook = br->copy(s / 9, s / 9);
    Fl_PNG_Image *wn = new Fl_PNG_Image("images/white_knight.png");
    images.white_knight = wn->copy(s / 9, s / 9);
    Fl_PNG_Image *bn = new Fl_PNG_Image("images/black_knight.png");
    images.black_knight = bn->copy(s / 9, s / 9);
    Fl_PNG_Image *wb = new Fl_PNG_Image("images/white_bishop.png");
    images.white_bishop = wb->copy(s / 9, s / 9);
    Fl_PNG_Image *bb = new Fl_PNG_Image("images/black_bishop.png");
    images.black_bishop = bb->copy(s / 9, s / 9);
    Fl_PNG_Image *wq = new Fl_PNG_Image("images/white_queen.png");
    images.white_queen = wq->copy(s / 9, s / 9);
    Fl_PNG_Image *bq = new Fl_PNG_Image("images/black_queen.png");
    images.black_queen = bq->copy(s / 9, s / 9);
    Fl_PNG_Image *wk = new Fl_PNG_Image("images/white_king.png");
    images.white_king = wk->copy(s / 9, s / 9);
    Fl_PNG_Image *bk = new Fl_PNG_Image("images/black_king.png");
    images.black_king = bk->copy(s / 9, s / 9);
    int w_disp = (w - s) / 2;
    int h_disp = (effectve_h - s) / 2;
    init_board(&board_);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boxes[i][j] = new Fl_Button(w_disp + (i * s) / 8, h_disp + (j * s) / 8 + 30, s / 8, s / 8);
            boxes[i][j]->box(FL_UP_BOX);
            indices[i][j] = (i << 4) + j;
            boxes[i][j]->callback(callbackfunc, &indices[i][j]);
            if ((i + j) % 2 == 0) {
                boxes[i][j]->color(fl_rgb_color(100, 100, 100));
            } else {
                boxes[i][j]->color(fl_rgb_color(0, 0, 200));
            };
        };
    };
    print_pieces_to_screen();

    window->end();
    window->show(argc, argv);
    return Fl::run();
};


 
// This defines the equivalent of main() for the current platform.

// int main() {


//     // board board_;
//     // move output_move;
//     // short promote_decision;
//     // bool stale_or_checkmated;
//     // init_board(&board_);
//     // // search_for_best_move(&board_, &output_move, &promote_decision, 3, &stale_or_checkmated);
//     // // std::cout << output_move.square_from[0] << "\n" << output_move.square_from[1] << "\n" << output_move.square_to[0] << "\n" << output_move.square_to[1] << "\n";
//     // // // move m1 = {{4, 0}, {0, 6}};
//     // // // move m2 = {{1, 1}, {1, 3}};
//     // // // move m3 = {{0, 3}, {1, 2}};
//     // // // make_move(&m1, &board_);
//     // // // make_move(&m2, &board_);
//     // // // make_move(&m3, &board_);
//     // check_and_pin_feedback feedback;
//     // // // reset_recent_move(&board_);
//     // // // reset_recent_move(&board_);
//     // // // reset_recent_move(&board_);
//     // // // printf("%d", *std::begin(feedback.valid_squares));
//     // // // printf("%s", feedback.valid_squares.count(7) ? "7" : "nah");
//     // // // printf("%s", feedback.valid_squares.count(0) ? "0" : "nah");
//     // // // print_board(&board_);
//     // short x0;
//     // short x1;
//     // short x2;
//     // short x3;
//     // move move_;
//     // print_board(&board_);
//     // while (true) {
//     //     is_checked_and_identify_pins_and_freedoms(&board_, board_.white_to_move, &feedback);
//     //     std::cin >> x0;
//     //     std::cin >> x1;
//     //     std::cin >> x2;
//     //     std::cin >> x3;
//     //     move_.square_from[0] = x0;
//     //     move_.square_to[0] = x2;
//     //     move_.square_from[1] = x1;
//     //     move_.square_to[1] = x3;
//     //     if (valid_move(&move_, &board_, &feedback)) {
//     //         make_move(&move_, &board_);
//     //         print_board(&board_);
//     //         search_for_best_move(&board_, &output_move, &promote_decision, 1, &stale_or_checkmated);
//     //         make_move(&output_move, &board_, promote_decision);
//     //         print_board(&board_);
//     //     } else {
//     //         printf("illegal\n");
//     //     };
//     // };
//     return 0;
// };