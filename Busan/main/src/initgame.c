#include <Busan/basedef/train.h>

#include <Busan/obj/citizen.h>
#include <Busan/obj/villain.h>

#include <stdlib.h>
 void __init_stage() {
#define cit_init(idx, pos) citizen(Pos, idx) = citizen(PosPrev, idx) = pos; citizen(Aggro, idx) = 1; train[citizen(Pos, idx)] = TRAIN_C;
    switch (stage) {
    case 1: citizenNum = 1; cit_init(0, trainLen - 6); break;
    case 2: {
        citizenNum = trainLen >> 2;
        for (int i = 0; i < citizenNum; i++) {
            int pos; do { pos = (rand() % (madongseok(Pos) - 2)) + 1; } while (train[pos] != TRAIN_0);
            cit_init(i, pos);
        }
    } break;
    default: {
        citizenNum = (trainLen >> 2) + (rand() % trainLen >> 2);
        for (int i = 0; i < citizenNum; i++) {
            int pos; do { pos = (rand() % madongseok(Pos) + (i ? citizen(Pos, i - 1) : i) - 2) + 1 + (i ? citizen(Pos, i - 1) : i); } while (train[pos] != TRAIN_0);
            cit_init(i, pos);
        }
    }
    }
#undef cit_init
}
void initGame() {
    for (int i = 0; i < trainLen; i++) train[i] = TRAIN_0;
    citizen_killed_init();
    __init_stage();
    zombie(PosPrev) = zombie(Pos) = trainLen - 2; train[zombie(Pos)] = TRAIN_Z;
    if (stage >= 2) {
        villain(Pos) = trainLen - 5; villain(Aggro) = 1; train[villain(Pos)] = TRAIN_V;
    }
    madongseok(Pos) = trainLen - 1; madongseok(Aggro) = 1; train[madongseok(Pos)] = TRAIN_M;
}