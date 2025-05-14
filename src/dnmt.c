#include "dnmt.h"
#include <stdio.h>

static u8 bit_rev(u8 x) {
    x = ((x & 0b11110000) >> 4) | ((x & 0b00001111) << 4);
    x = ((x & 0b11001100) >> 2) | ((x & 0b00110011) << 2);
    x = ((x & 0b10101010) >> 1) | ((x & 0b01010101) << 1);
    return x >> 1;
}

Board from_string(const char* str) {
    Board b = {};
    u64 bit_index = 0;
    for (u32 str_index = 0; str[str_index] != 0;) {
        char c = str[str_index++];

        if (bit_index % 8 == 7) {
            bit_index++;
        }

        if (c == 'x' || c == 'X') {
            b.yel |= 1ull << bit_index;
        }
        if (c == 'o' || c == 'O') {
            b.red |= 1ull << bit_index;
        }
        bit_index++;
    }
    return b;
}

GameResult play_game(const Player* red, const Player* yellow, bool print) {
    // initialize both players
    void* red_ctx = red->init();
    void* yellow_ctx = yellow->init();

    Board b = {};

    while (board_can_place_anywhere(b)) {
        // ask red for a placement
        u8 place_column = red->place(red_ctx, b.red, b.yel);
        if (print) printf("red: %c\n", place_column + 'a');
        // see if it actually works
        if (!board_can_place(b, place_column)) {
            return GR_RED_INVALID;
        }
        // place it
        b.red |= board_place_pos(b, place_column);
        if (print) board_print(b);
        // check for a win
        u64 win_board = bitboard_wins(b.red);
        if (win_board != 0) {
            return GR_RED_WIN;
        }

        if (!board_can_place_anywhere(b)) {
            // no moves left for yellow
            break;
        }

        // ask yellow for a placement
        place_column = yellow->place(yellow_ctx, b.yel, b.red);
        if (print) printf("yellow: %c\n", place_column + 'a');
        // see if it actually works
        if (!board_can_place(b, place_column)) {
            return GR_YELLOW_INVALID;
        }
        // place it
        b.yel |= board_place_pos(b, place_column);
        if (print) board_print(b);
        // check for a win
        win_board = bitboard_wins(b.yel);
        if (win_board != 0) {
            return GR_YELLOW_WIN;
        }
    }

    return GR_DRAW;
}

int main() {
    GameResult gr = play_game(&player_user, &player_random, true);
    switch (gr) {
    case GR_DRAW:
        printf("draw!\n");
        break;
    case GR_RED_WIN:
        printf("red wins!\n");
        break;
    case GR_YELLOW_WIN:
        printf("yellow wins!\n");
        break;
    case GR_RED_INVALID:
        printf("red made an invalid move!\n");
        break;
    case GR_YELLOW_INVALID:
        printf("yellow made an invalid move!\n");
        break;
    }
}