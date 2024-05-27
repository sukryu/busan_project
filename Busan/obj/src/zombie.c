#include <Busan/basedef/global.h>
#include <Busan/basedef/train.h>

#include <Busan/obj/zombie.h>
#include <Busan/obj/citizen.h>
#include <Busan/obj/villain.h>

#include <stdlib.h>
#include <stdio.h>

Busan_errno_t zombie_moveCond(int will, int turn) {
    Busan_errno_t err;
    int rand_val = rand() % 100;
    printf("Random Value: %d, Probability: %d\n", rand_val, p); // 디버그 출력

    // 짝수 턴에서만 이동
    if (turn % 2 != 0) {
        printf("Zombie cannot move on odd turns.\n");
        return ZOMBIE_IDLE;
    }

    // 확률 체크
    if (rand_val >= (100 - p)) {
        printf("Zombie did not move due to probability check.\n");
        return ZOMBIE_IDLE;
    }

    // 객체 이동 조건 확인
    switch (err = obj_moveCond(&zombie, will)) {
        case OBJ_GOOD: 
            break;
        default: 
            printf("Zombie move condition failed with error: %d\n", err);
            return err + ZOMBIE_LOCALE_OBJ;
    }

    // 동일한 위치로 이동할 수 없도록 추가 로직
    if (zombie(Pos) + will == madongseok(Pos) || 
        (zombie(Pos) + will >= 0 && train[zombie(Pos) + will] == TRAIN_C)) {
        printf("Zombie move blocked by Madongseok or train.\n");
        return ZOMBIE_IDLE; // 이동하지 않음
    }

    return ZOMBIE_GOOD;
}

Busan_errno_t zombie_move(int turn) { 
    int will;
    int max_citizen_aggro = citizen_aggroMax(); // 최대 어그로를 가진 시민의 인덱스를 가져옴
    
    printf("M_p: %d, M_A: %d, C_A: %d, Z_p: %d\n", madongseok(Pos), madongseok(Aggro), citizen(Aggro, max_citizen_aggro), zombie(Pos));

    if (madongseok(Aggro) >= citizen(Aggro, max_citizen_aggro)) {
        will = madongseok(Pos) > zombie(Pos) ? 1 : -1; // 마동석 쪽으로 이동
    } else {
        will = citizen(Pos, max_citizen_aggro) > zombie(Pos) ? 1 : -1; // 어그로 높은 시민 쪽으로 이동
    }
    
    printf("Zombie Will: %d\n", will); // 이동 방향 출력
    
    Busan_errno_t cond = zombie_moveCond(will, turn);
    printf("cond: %d\n", cond); // 이동 조건 확인
    switch (cond) {
        case ZOMBIE_GOOD: {
            printf("Zombie is moving from %d to %d\n", zombie(Pos), zombie(Pos) + will);
            return ZOMBIE_LOCALE_OBJ + obj_move(&zombie, will);
        }
        case ZOMBIE_IDLE:
            printf("Z: stay %d\n\n", zombie(Pos));
            return ZOMBIE_GOOD;
        default:
            printf("Zombie move error: %d\n", cond);
            break;
    }
    return cond;
}

#define ZOMBIE_TAR_VILLAIN -1
#define ZOMBIE_TAR_M -2

int zombie_target() {
    int maxIdx = citizen_aggroMax();
    if (citizen(Aggro, maxIdx) < madongseok(Aggro)) {
        return ZOMBIE_TAR_M;
    } else {
        return maxIdx;
    }
}

void zombie_action() {
    int tar = zombie_target();
    printf("Zombie target: %d\n", tar);
    switch (tar) {
    case ZOMBIE_TAR_VILLAIN: {
        if ((zombie(Pos) == madongseok(Pos) - 1 || zombie(Pos) == madongseok(Pos) + 1)) {
            printf("Zombie attacks villain.\n");
            villain_kill();
        }
    } break;
    case ZOMBIE_TAR_M: {
        if ((zombie(Pos) == madongseok(Pos) - 1 || zombie(Pos) == madongseok(Pos) + 1)) {
            printf("Zombie attacks Madongseok.\n");
            madongseok(Stamina)--;
        }
    } break;
    default: {
        if ((zombie(Pos) == citizen(Pos, tar) - 1 || zombie(Pos) == citizen(Pos, tar) + 1)) {
            printf("Zombie attacks citizen %d.\n", tar);
            citizen_kill(tar);
        }
    } break;
    }
}

