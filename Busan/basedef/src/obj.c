#include <Busan/basedef/train.h>
#include <Busan/basedef/const.h>
#include <Busan/basedef/fun.h>

#include <Busan/basedef/obj.h>

#include <stdio.h>
#include <stdlib.h>


void obj_printMove(ptr_obj p) {
    switch (p->id) {
    case TRAIN_C: printf("C"); break;
    case TRAIN_EZ: printf("Z"); break;
    case TRAIN_M: printf("M"); break;
    case TRAIN_V: printf("V"); break;
    case TRAIN_Z: printf("Z"); break;
    } printf(": ");

    if (p->PosPrev != p->Pos) {
        printf("%d -> %d", p->PosPrev, p->Pos);
    }
    else {
        printf("stay %d", p->Pos);
    }

    switch (p->id) {
    default:
        printf(" (aggro: %d -> %d)", p->AggroPrev, p->Aggro);
    case TRAIN_Z: case TRAIN_EZ:
        printf("\n");
        break;
    }
}

Busan_errno_t obj_moveCond(ptr_obj p, char will) {
    if (p->Pos + will >= trainLen || p->Pos + will < 0) {
        printf("Move out of range: %d -> %d\n", p->Pos, p->Pos + will);
        return OBJ_OUT_OF_RANGE;
    }

    switch (train[p->Pos + will]) {
    case TRAIN_0: break;
    default: 
        printf("Move blocked by object at %d\n", p->Pos + will);
        return OBJ_BLOCKED;
    }

    return OBJ_GOOD;
}

Busan_errno_t obj_move(ptr_obj p, int will) {
    Busan_errno_t err = obj_moveCond(p, will);

    switch (err) {
    case OBJ_GOOD:
        printf("Object moving from %d to %d\n", p->Pos, p->Pos + will);
        train[p->PosPrev = p->Pos] = TRAIN_0;
        train[p->Pos += will] = p->id;
        break;
    default: 
        printf("Object move failed with error: %d\n", err);
        break;
    }
    return err;
}


void obj_aggro(ptr_obj p, int will) {
    p->AggroPrev = p->Aggro;
    p->Aggro += will;
}