#pragma once
#include <Busan/basedef/obj.h>

#ifndef ZOMBIE_H
#define ZOMBIE_H

enum err_zombie {
	ZOMBIE_GOOD,
	ZOMBIE_IDLE,
	ZOMBIE_M_CORRUPTED,

	ZOMBIE_LOCALE_OBJ,
	ZOMBIE_LOCALE_OBJ_LMT = ZOMBIE_LOCALE_OBJ + OBJ_LMT,
	ZOMBIE_LMT = ZOMBIE_LOCALE_OBJ_LMT,
};

Busan_errno_t zombie_move();
void zombie_action();

#endif // !ZOMBIE_H
