#include <time.h>
#include <stdlib.h>

#include "../dnmt.h"

static f32 eval(void* ctx, u64 player, u64 opponent) {
    return 0;
}

static u8 place(void* ctx, u64 player, u64 opponent) {
    u8 place_col = rand() % 7;
    Board b = {player, opponent};
    while (!board_can_place(b, place_col)) {
        place_col = rand() % 7;
    }
    return place_col;
}

static void* init() {
    static time_t t = 0;
    if (t == 0) {
        time(&t);
        srand(t);
    }
    return NULL;
}

const Player player_random = {
    .name = "random",
    .eval = eval,
    .place = place,
    .init = init,
};