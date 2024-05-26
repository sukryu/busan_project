#include <Busan/basedef/const.h>
#include <Busan/basedef/global.h>

#include <Busan/obj/madongseok.h>
#include <stdlib.h>
#include <stdio.h>

 Busan_errno_t madongseok_moveCond() {
    if (madongseok(Direction) == MOVE_STAY) return M_IDLE;
    if (madongseok(Pos) <= 1) return M_OUT;
    Busan_errno_t err; 
    switch (err = obj_moveCond(&madongseok, -1)) {
    case OBJ_GOOD: break;
    default: return err + M_LOCALE_OBJ;
    } return M_GOOD;
}
Busan_errno_t madongseok_move() {
    Busan_errno_t err; switch (err = madongseok_moveCond()) {
    case M_GOOD: {
        obj_aggro(&madongseok, (madongseok(Aggro) < AGGRO_MAX));
        obj_move(&madongseok, -1);
        break;
    default:
        obj_aggro(&madongseok, -(madongseok(Aggro) > AGGRO_MIN));
        break;
    }
    } return err;
}

void madongseok_stamina(int will) {
    madongseokStaminaPrev = madongseokStamina;
    madongseokStamina += will;
}

int madonseok_action(int action) {
    switch (action) {
    case ACTION_REST:
        madongseok(Stamina) += madongseok(Stamina) < STM_MAX;
        obj_aggro(&madongseok, -(madongseok(Aggro) > AGGRO_MIN));
        printf("madongseok rests...\n\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d\n)",
            madongseok(Pos), madongseok(AggroPrev), madongseok(Aggro), madongseokStaminaPrev, madongseokStamina);
        break;
    case ACTION_PROVOKE:
        madongseok(Stamina) = AGGRO_MAX;
        printf("madongseok provoked zombie...\n\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d\n)",
            madongseok(Pos), madongseok(AggroPrev), madongseok(Aggro), madongseokStaminaPrev, madongseokStamina);
        break;
    case ACTION_PULL:
        if (zombie(Pos) == madongseok(Pos) - 1) {
            obj_aggro(&madongseok, (madongseok(Aggro) + 2 <= AGGRO_MAX) << 1);
            madongseok(Stamina) -= madongseok(Stamina) > STM_MIN;
            int isSuccess = (rand() % 100 >= p);
            if (isSuccess) 
        }
    default: break;
    }
    return 0;
}