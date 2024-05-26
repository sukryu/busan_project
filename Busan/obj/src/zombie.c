#include <Busan/basedef/global.h>
#include <Busan/basedef/train.h>

#include <Busan/obj/zombie.h>
#include <Busan/obj/citizen.h>
#include <Busan/obj/villain.h>

#include <stdlib.h>
#include <stdio.h>

 Busan_errno_t zombie_moveCond(int will) {
    Busan_errno_t err;
    if (rand() % 100 > p) return ZOMBIE_IDLE;
    switch (err = obj_moveCond(&zombie, will)) {
    case OBJ_GOOD: break;
    default: return err + ZOMBIE_LOCALE_OBJ;
    }

    if (zombie(Pos) + will >= madongseokPos)
        return ZOMBIE_M_CORRUPTED;

    return ZOMBIE_GOOD;
}
Busan_errno_t zombie_move() {
    const int will = madongseokAggro >= citizen_aggroMax() ? 1 : -1;
    Busan_errno_t cond = zombie_moveCond(will);
    switch (cond) {
    case ZOMBIE_GOOD: {
        return ZOMBIE_LOCALE_OBJ + obj_move(&zombie, will);
    } default: break;
    }

    return cond;
}

#define ZOMBIE_TAR_VILLAIN -1
#define ZOMBIE_TAR_M -2

 int zombie_target() {
    int maxIdx = citizen_aggroMax();
    if (citizen(Aggro, maxIdx) < villain(Aggro)) {
        return ZOMBIE_TAR_VILLAIN;
    }
    else if (citizen(Aggro, maxIdx) < madongseok(Aggro)) {
        return ZOMBIE_TAR_M;
    }
    else return maxIdx;
}

void zombie_action() {
    int tar = zombie_target();
    switch (tar) {
    case ZOMBIE_TAR_VILLAIN: {
        // zombie dunked on
        if((zombie(Pos) == madongseok(Pos) - 1 || zombie(Pos) == madongseok(Pos) + 1))
            villain_kill();
    } break;
    case ZOMBIE_TAR_M: {
        // stupid villain killed
        if ((zombie(Pos) == madongseok(Pos) - 1 || zombie(Pos) == madongseok(Pos) + 1))
            madongseok(Stamina)--;
    } break;
    default: {
        // zombie do shit
        if ((zombie(Pos) == citizen(Pos, tar) - 1 || zombie(Pos) == citizen(Pos, tar) + 1))
            citizen_kill(tar);
    } break;
    }
}