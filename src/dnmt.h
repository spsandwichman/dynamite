#ifndef C4_H
#define C4_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <x86intrin.h>


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t  i8;
typedef uint16_t i16;
typedef uint32_t i32;
typedef uint64_t i64;

typedef float  f32;
typedef double f64;

#define for_n(n, start, end) for (i64 n = start; n < end; ++n)

/* i pretend each row is 8-wide (even tho its 7) so that each row is a byte
                            (fake)
    00 01 02 03 04 05 06    (07)
    08 09 10 11 12 13 14    (15)
    16 17 18 19 20 21 22    (23)
    24 25 26 27 28 29 30    (31)
    32 33 34 35 36 37 38    (39)
    40 41 42 43 44 45 46    (47)
*/

#define BOARD       0x7f7f7f7f7f7full
#define COL         0x010101010101ull
#define COL_BITS(n) (COL << (n))
#define ROW         0x00000000007full
#define ROW_BITS(n) (ROW << (n * 8))

typedef struct Board {
    u64 red;
    u64 yel;
} Board;

void board_print(Board b);
void bitboard_print(u64 bits);
u64 board_place_pos(Board b, u8 column);
bool board_can_place(Board b, u8 column);
bool board_can_place_anywhere(Board b);
bool bitboard_check_win(u64 b);
u64 bitboard_wins(u64 b);
u64 board_colorless_pieces(u64 player, u64 opponent);

typedef enum {
    GR_DRAW,
    GR_RED_WIN,
    GR_YELLOW_WIN,
    GR_RED_INVALID,
    GR_YELLOW_INVALID,
} GameResult;

typedef struct Player {
    const char* name;
    f32   (*eval)(void* context, u64 player, u64 opponent);
    u8    (*place)(void* context, u64 player, u64 opponent); // returns column for placement
    void* (*init)();
} Player;

extern const Player player_random;
extern const Player player_user;

#endif // C4_H