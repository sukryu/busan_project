#pragma once
#include <Busan/basedef/obj.h>
#ifndef MADONGSEOK_H
#define MADONGSEOK_H

enum err_madongseok {
    M_GOOD,
    M_IDLE,
    M_OUT,

    M_LOCALE_OBJ,
    M_LOCALE_OBJ_LMT = M_LOCALE_OBJ + OBJ_LMT,
    M_LMT = M_LOCALE_OBJ_LMT
};

Busan_errno_t madongseok_move();
void madonseok_action(int);

#endif // !MADONGSEOK_H
