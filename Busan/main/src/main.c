#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <Busan/basedef/const.h>
#include <Busan/basedef/global.h>
#include <Busan/basedef/train.h>
#include <Busan/basedef/fun.h>

#include <Busan/obj/citizen.h>
#include <Busan/obj/madongseok.h>
#include <Busan/obj/villain.h>
#include <Busan/obj/zombie.h>

#define scanf_s scanf
#define printTab() printf("\n\n")

int main() {
    srand((unsigned int)time(NULL));

    bool isGrapped = false;

    while (1) {
        printf("train length(%d ~ %d)>> ", LEN_MIN, LEN_MAX);
        scanf_s("%d", &trainLen);
        if (trainLen >= LEN_MIN && trainLen <= LEN_MAX) break;
    } train[trainLen] = 0;
    while (1) {
        printf("madonseok stamina(%d ~ %d)>> ", STM_MIN, STM_MAX);
        scanf_s("%d", &madongseok(Stamina));
        if (madongseok(Stamina) >= STM_MIN && madongseok(Stamina) <= STM_MAX) break;
    }
    while (1) {
        printf("percentile probability 'p'(%d ~ %d)>> ", PROB_MIN, PROB_MAX);
        scanf_s("%d", &p);
        if (p >= PROB_MIN && p <= PROB_MAX) break;
    }

    printTab();

    for (stage = 1; stage <= 4; stage++) {
        turn = 0;
        initGame();
        printTrain();
        // savePrevPositions();
        printTab();

        printf("Stage %d start!\n", stage);

        while (1) {
            turn++;
            citizen_move();
            if (stage >= 2) villain_move();
            if (turn % 2 != 0 && !isGrapped) zombie_move();

            printf("Current Turn: %d\n", turn);
            printTrain();
            printTab();
            printStatus();
            printTab();

            while (1) {

                if ((madongseok(Pos) - 1) == zombie(Pos)) {
                    printf("madonseok move(0:stay)>> ");
                    scanf_s("%d", &madongseokDirection);
                    if (madongseokDirection == MOVE_STAY) break;
                }
                else {
                    printf("madonseok move(0:stay, 1:left)>> ");
                    scanf_s("%d", &madongseokDirection);
                    if (madongseokDirection == MOVE_LEFT || madongseokDirection == MOVE_STAY) break;
                }
            }

            if (madongseok_move() != M_GOOD) {
                printTrain();
                printf("\n\n");
                printf("madongseok stay %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok(Pos), madongseok(AggroPrev), madongseok(Aggro), madongseokStaminaPrev, madongseokStamina);
            } else {
                printTrain();
                printf("\n\n");
                printf("madongseok move %d -> %d");
            }

            citizen_action();
            if (stage >= 2) {
                villain_action();
                printf("villain ");
                if (train[villain(Pos)] == TRAIN_C) {
                    printf("swapped position with citizen.\n");
                }
                else {
                    printf("did nothing.\n");
                }
            }
            else {
                printf("citizen does nothing.\n");
            }

            zombie_action();

            bool citizenAttacked = 0;
            for (int i = 0; i < citizenNum; i++) {
                if (train[citizen(Pos, i)] == TRAIN_Z || train[citizen(Pos, i)] == TRAIN_EZ) {
                    citizenAttacked = 1;
                    break;
                }
            }

            if (citizenAttacked) {
                printf("zombie attacked citizen. \n");
            }
            else if (train[madongseok(Pos)] == TRAIN_Z + 1 || train[madongseok(Pos)] == TRAIN_EZ + 1) {
                printf("zombie attacked madongseok. \n");
            }
            else {
                printf("zombie attacked nobody.\n");
            }

            printf("\n\n");

            int dongseokAction;
            while (1) {
                printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
                scanf_s("%d", &dongseokAction);
                if (dongseokAction >= ACTION_REST && dongseokAction <= ACTION_PULL) break;
            }
            printf("\n\n");
            isGrapped = madonseok_action(dongseokAction);

            if (citizenNum == 0) {
                printf("GAME OVER! All citizens are dead.\n");
                return 0;
            }

            int allEscaped = 1;
            for (int i = 0; i < citizenNum; i++) {
                if (citizen(Pos, i) != 0) {
                    allEscaped = 0;
                    break;
                }
            }   
            if (allEscaped) break;

            if (madongseok(Stamina) <= 0) {
                printf("GAME OVER! Madongseok is exhausted. \n");
                return 0;
            }
        }
    }
    printf("YOU WIN! All citizens escaped safely.\n");
}