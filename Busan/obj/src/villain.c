#include <Busan/basedef/train.h>
#include <Busan/basedef/const.h>
#include <Busan/basedef/global.h>

#include <Busan/obj/villain.h>

#include <stdbool.h>
#include <stdlib.h>

 Busan_errno_t villain_moveCond() {
    Busan_errno_t err; switch (err = obj_moveCond(&villain, 1)) {
    case OBJ_GOOD: break;
    default: return err + VILL_LOCALE_OBJ;
    }

    char civOnRight = 0; for (int i = 0; i < citizenNum; i++)
        civOnRight = civOnRight || citizen(Pos, i) > villain(Pos);

    if (!civOnRight) return VILL_IDLE;

    return VILL_GOOD;
}
Busan_errno_t villain_move() {
    Busan_errno_t rtn; switch (rtn = villain_moveCond()) {
    case VILL_GOOD: 
        obj_aggro(&villain, villain(Aggro) < AGGRO_MAX);
        break;
    default:
        obj_aggro(&villain, -(villain(Aggro) > AGGRO_MIN));
    }
    return rtn;
}
void villain_kill() {
    train[villain(Pos)] = TRAIN_0;
    villain(Pos) = -1;
}

 bool __villain_action(int i) {
    if (villain(Pos) == citizen(Pos, i) - 1 || villain(Pos) == citizen(Pos, i) + 1) {
        if (rand() % 100 < 30) {
            int temp = citizen(Pos, i);
            citizen(Pos, i) = villain(Pos);
            villain(Pos) = temp;

            temp = citizen(Aggro, i);
            citizen(Aggro, i) = villain(Aggro);
            villain(Aggro) = temp;

            train[citizen(Pos, i)] = TRAIN_C;
            train[villain(Pos)] = TRAIN_V;
        }
        return false;
    } 
    
    return true;
}

void villain_action() {
    bool proceed = 1;
    for (int i = 0; i < citizenNum && proceed; i++) 
        if(citizen(id, i) == TRAIN_C)
            proceed = __villain_action(i);

}