#pragma once
#include <Busan/basedef/obj.h>

#ifndef CITIZEN_H
#define CITIZEN_H

enum err_citizen {
    CITIZEN_GOOD,
    CITIZEN_IDLE,

    CITIZEN_INFECTED,

    CITIZEN_LOCALE_OBJ,
    CITIZEN_LOCALE_OBJ_LMT = CITIZEN_LOCALE_OBJ + OBJ_LMT,

    CITIZEN_LMT = CITIZEN_LOCALE_OBJ_LMT
};

Busan_errno_t citizen_move();
int citizen_aggroSum();
int citizen_aggroMax();
void citizen_action();
Busan_errno_t citizen_kill(int idx);

int citizen_killed();
void citizen_killed_init();

#endif // !CITIZEN_H