#include "c4.h"

static u8 bit_rev(u8 x) {
    x = ((x & 0b11110000) >> 4) | ((x & 0b00001111) << 4);
    x = ((x & 0b11001100) >> 2) | ((x & 0b00110011) << 2);
    x = ((x & 0b10101010) >> 1) | ((x & 0b01010101) << 1);
    return x >> 1;
}

u64 from_visual(u64 r1, u64 r2, u64 r3, u64 r4, u64 r5, u64 r6) {
    r1 = bit_rev(r1) & 0b01111111;
    r2 = bit_rev(r2) & 0b01111111;
    r3 = bit_rev(r3) & 0b01111111;
    r4 = bit_rev(r4) & 0b01111111;
    r5 = bit_rev(r5) & 0b01111111;
    r6 = bit_rev(r6) & 0b01111111;
    return r1 | (r2 << 8) | (r3 << 16) | (r4 << 24) | (r5 << 32) | (r6 << 40);
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

int main() {
    Board b = {};
    b = from_string(""
        "OOXO..."
        "XOXX.OX"
        "OOXO.OX"
        "XXOO.XO"
        "OOOXXOO"
        "XXXOXXX"
    );
    board_print(b);

    u64 colorless = board_colorless_pieces(b.red, b.yel);
    bitboard_print(colorless);
    b.red &= ~colorless;
    b.yel &= ~colorless;
    board_print(b);
    
    // u64 place = board_place_pos(b, 0);
    // bitboard_print(place);
    // printf("%016zx\n", place);
}