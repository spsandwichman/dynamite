#include "dnmt.h"

#define Reset "\x1b[0m"
#define Bold "\x1b[1m"
#define Dim "\x1b[2m"
#define Italic "\x1b[3m"
#define Underline "\x1b[4m"

#define Red "\x1b[31m"
#define Green "\x1b[32m"
#define Yellow "\x1b[33m"

void bitboard_print(u64 bits) {
    printf("╭╴"Bold"a"Reset"-"Bold"b"Reset"-"Bold"c"Reset"-"Bold"d"Reset"-"
            Bold"e"Reset"-"Bold"f"Reset"-"Bold"g"Reset"╶╮\n");
    for_n(i, 0, 6) {
        printf("│ ");
        for_n(j, 0, 7) {
            u64 bit = COL_BITS(j) & ROW_BITS(i);
            if (bit & bits) {
                printf(Green Bold "x "Reset);
            } else {
                printf(Dim". "Reset);
            }
        }
        printf("│\n");
    }
    printf("╰───────────────╯\n");
}

void board_print(Board b) {
    printf("╭╴"Bold"a"Reset"-"Bold"b"Reset"-"Bold"c"Reset"-"Bold"d"Reset"-"
            Bold"e"Reset"-"Bold"f"Reset"-"Bold"g"Reset"╶╮\n");
    for_n(i, 0, 6) {
        printf("│ ");
        for_n(j, 0, 7) {
            u64 bit = COL_BITS(j) & ROW_BITS(i);
            if (bit & b.red) {
                printf(Red Bold "o " Reset);
            } else if (bit & b.yel) {
                printf(Yellow Bold "x " Reset);
            } else {
                printf(Dim". "Reset);
            }
        }
        printf("│\n");
    }
    printf("╰───────────────╯\n");
}

static inline u64 and(u64 b, u64 stride) {
    u64 x = b & (b >> stride);
    return x & (x >> (2 * stride));
}

static inline u64 or(u64 b, u64 stride) {
    u64 x = b | (b << stride);
    return x | (x << (2 * stride));
}

u64 bitboard_wins(u64 b) {
    b &= BOARD;
    u64 v = or(and(b, 8), 8);
    u64 h = or(and(b, 1), 1);
    u64 d1 = or(and(b, 7), 7);
    u64 d2 = or(and(b, 9), 9);
    u64 wins = v | h | d1 | d2;
    return wins;
}

bool bitboard_check_win(u64 b) {
    u64 v = and(b, 8);
    u64 h = and(b, 1);
    u64 d1 = and(b, 7);
    u64 d2 = and(b, 9);
    u64 win = v | h | d1 | d2;
    // bitboard_print(win);
    return win;
}

u64 board_colorless_pieces(u64 player, u64 opponent) {
    u64 potential_player = bitboard_wins(BOARD & ~opponent);
    u64 potential_opponent = bitboard_wins(BOARD & ~player);
    return (player | opponent) & ~potential_player & ~potential_opponent;
}

u64 board_place_pos(Board b, u8 column) {
    u64 occupied = (b.red | b.yel);
    u64 occupied_in_col = occupied & COL_BITS(column);
    if (occupied_in_col) {
        return (1ull << (__builtin_ctzll(occupied_in_col) - 8)) & BOARD;
    }
    return (1ull << (column + 40));
}

bool board_can_place(Board b, u8 column) {
    u64 occupied = (b.red | b.yel);
    return ~occupied & (1ull << column);
}

bool board_can_place_anywhere(Board b) {
    u64 occupied = (b.red | b.yel);
    return ~occupied & ROW_BITS(0);
}