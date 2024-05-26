#include <stdio.h>

#include <Busan/basedef/global.h>
#include <Busan/basedef/train.h>
#include <Busan/basedef/const.h>


void printTrain() {
    for (int i = 0; i < trainLen + 2; i++) printf("#");
    printf("\n#%s#\n", train);
    for (int i = 0; i < trainLen + 2; i++) printf("#");
    printf("\n");
}
void printStatus() {
    for (int i = 0; i < citizenNum; i++) {
        if (citizen(Pos, i) >= 0) {
            obj_printMove(citizen + i);
        }
    }
    if (zombie(Pos) >= 0) {
        obj_printMove(&zombie);
    }
}