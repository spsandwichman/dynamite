#include <stdio.h>
#include "../dnmt.h"


static f32 eval(void* ctx, u64 player, u64 opponent) {
    return 0;
}

static u8 place(void* ctx, u64 player, u64 opponent) {
    printf("> ");
    char in[1000];
    fgets(in, sizeof(in), stdin);
    return in[0] - 'a';
}

static void* init() {
    return NULL;
}

const Player player_user = {
    .name = "user",
    .eval = eval,
    .place = place,
    .init = init,
};