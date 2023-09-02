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
#include <chrono>

Fl_Button *boxes[8][8];
Fl_Button *promotion_options[4];
int indices[8][8];
int promotion_indices[4] = {0, 1, 2, 3};

move move_;
bool first_move = true;
bool waiting_for_promotion_decision = false;
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
        if (!waiting_for_promotion_decision) {
            move_.square_to[0] = i;
            move_.square_to[1] = j;
            is_checked_and_identify_pins_and_freedoms(&board_, board_.white_to_move, &user_check_pin_feedback);
            if (valid_move(&move_, &board_, &user_check_pin_feedback)) {
                if ((board_.white_to_move && board_.piece_positions[move_.square_from[0]][move_.square_from[1]] == white_pawn && move_.square_to[1] == white_promote_row) ||
                 (!board_.white_to_move && board_.piece_positions[move_.square_from[0]][move_.square_from[1]] == black_pawn && move_.square_to[1] == black_promote_row)) {
                    waiting_for_promotion_decision = true;
                    for (int i = 0; i < 4; i++) {
                        promotion_options[i]->show();
                    };
                    return;
                 } else {
                    make_move(&move_, &board_);
                    print_pieces_to_screen();
                };
            };
        } else {
            waiting_for_promotion_decision = false;
            for (int i = 0; i < 4; i++) {
                promotion_options[i]->box(FL_NO_BOX);
            };
        };
        first_move = true;
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

void promote_callbackfunc(Fl_Widget * widget, void * data) {
    int index = *(int*)data;
    first_move = true;
    if (board_.white_to_move) {
        switch (index) {
            case 0:
                make_move(&move_, &board_, white_queen);
                break;
            case 1:
                make_move(&move_, &board_, white_rook);
                break;
            case 2:
                make_move(&move_, &board_, white_bishop);
                break;
            case 3:
                make_move(&move_, &board_, white_knight);
                break;
        };
    } else {
        switch (index) {
            case 0:
                make_move(&move_, &board_, black_queen);
                break;
            case 1:
                make_move(&move_, &board_, black_rook);
                break;
            case 2:
                make_move(&move_, &board_, black_bishop);
                break;
            case 3:
                make_move(&move_, &board_, black_knight);
                break;
        };
    };
    print_pieces_to_screen();
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
    for (int i = 0; i < 4; i++) {
        promotion_options[i]->hide();
    };
};

int main(int argc, char *argv[]) {
    bool player_is_white = std::chrono::system_clock::now().time_since_epoch().count() % 2 == 0;
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
            if (player_is_white) {
                boxes[i][j] = new Fl_Button(w_disp + (i * s) / 8, h_disp + ((7 - j) * s) / 8 + 30, s / 8, s / 8);
            } else {
                boxes[i][j] = new Fl_Button(w_disp + (i * s) / 8, h_disp + (j * s) / 8 + 30, s / 8, s / 8);
            };
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
    for (int i = 0; i < 4; i++) {
        promotion_options[i] = new Fl_Button(w_disp + (i * s) / 4, h_disp + 30, s / 4, s / 4);
        promotion_options[i]->box(FL_UP_BOX);
        if (player_is_white) {
            promotion_options[i]->color(fl_rgb_color(0, 0, 0));
        } else {
            promotion_options[i]->color(fl_rgb_color(255, 255, 255));
        };
        promotion_options[i]->hide();
        promotion_options[i]->callback(promote_callbackfunc, &promotion_indices[i]);
    };
    if (player_is_white) {
        promotion_options[0]->image(images.white_queen);
        promotion_options[1]->image(images.white_rook);
        promotion_options[2]->image(images.white_bishop);
        promotion_options[3]->image(images.white_knight);
    } else {
        promotion_options[0]->image(images.black_queen);
        promotion_options[1]->image(images.black_rook);
        promotion_options[2]->image(images.black_bishop);
        promotion_options[3]->image(images.black_knight);
    };
    print_pieces_to_screen();

    window->end();
    window->show(argc, argv);
    return Fl::run();
};
