#pragma once
#include <Busan/basedef/obj.h>

#ifndef VILLAIN_H
#define VILLAIN_H

enum err_villain {
    VILL_GOOD,
    VILL_IDLE,

    VILL_LOCALE_OBJ,
    VILL_LOCALE_OBJ_LMT = VILL_LOCALE_OBJ + OBJ_LMT,

    VILL_LMT = VILL_LOCALE_OBJ_LMT
};

Busan_errno_t villain_move();
void villain_kill();
void villain_action();
#endif // !VILLAIN_H
