#include <Busan/basedef/global.h>
#include <Busan/basedef/const.h>
#include <Busan/basedef/train.h>

#include <Busan/obj/citizen.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

 Busan_errno_t citizen_moveCond(ptr_obj o) {
    if (rand() % 100 > p) return CITIZEN_IDLE;
    Busan_errno_t err = obj_moveCond(o, -1);
    switch (err) {
    case OBJ_GOOD: break;
    default: return err + CITIZEN_LOCALE_OBJ;
    }
    return CITIZEN_GOOD;
}
Busan_errno_t citizen_move() {
    for (int i = 0; i < citizenNum; i++) {
        Busan_errno_t err;
        citizen(AggroPrev, i) = citizen(Aggro, i);

        switch (err = citizen_moveCond(citizen + i)) {
        case CITIZEN_GOOD:
            obj_move(citizen + i, -1);
            obj_aggro(citizen + i, (citizen(Aggro, i) < AGGRO_MAX));
            break;
        default:
            obj_aggro(citizen + i, -(citizen(Aggro, i) > AGGRO_MIN));
            break;
        } return err;
    }
}
int citizen_aggroSum() {
    int rtn = 0;
    for (int i = 0; i < citizenNum; i++) rtn += citizen(Aggro, i);
    return rtn / citizenNum;
}
int citizen_aggroMax() {
    int rtn = 0;

    for (int i = 1; i < citizenNum; i++)
        if (citizen(Aggro, i) > citizen(Aggro, rtn))
            rtn = i;

    return rtn;
}
int _citizenKilled(int off) {
    static int rtn = 0;
    return (rtn += off);
}
int citizen_killed() {
    return _citizenKilled(0);
}
void citizen_killed_init() {
    int k = citizen_killed();
    _citizenKilled(-citizen_killed());
}
Busan_errno_t citizen_kill(int i) {
    printf("citizen%d has been attacked by zombie.\n%d citizen(s) alive.\n\n", i, citizenNum - 1);

    switch (stage) {
    case 4:
        train[citizen(Pos, i)] = citizen(id, i) = TRAIN_EZ; // Enhanced Zombie: 5
        printf("citizen%d turned into an enhanced zombie!\n", i);
        return CITIZEN_INFECTED;
    default:
        train[citizen(Pos, i)] = TRAIN_0;   // just dead
        citizen(id, i) = TRAIN_0;           // blank, dead
    }

    _citizenKilled(1);
    return CITIZEN_GOOD;
}
void citizen_action() {
    for (int i = 0; i < citizenNum; i++) {
        if (citizen(Pos, i) == TRAIN_0) {
            train[citizen(Pos, i)] = TRAIN_0;
            printf("citizen%d has escaped.\n", i);
            for (int j = i; j < citizenNum - 1; j++) {
                citizen(Pos, j) = citizen(Pos, j + 1);
                citizen(Aggro, j) = citizen(Aggro, j + 1);
            }
            citizenNum--;
            i--;
        }
    }
}