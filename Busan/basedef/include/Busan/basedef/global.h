#pragma once

#include "obj.h"

#ifndef GLOBAL_H
#define GLOBAL_H
extern char train[];

extern char stage;
extern int trainLen;
extern struct obj citizen[];

extern int citizenNum;
#define citizen(mem, idx) citizen[idx].mem

extern struct obj zombie;
#define zombie(mem) zombie . mem

extern struct obj villain;
#define villain(mem) villain . mem

extern struct obj madongseok;

extern int madongseokStamina;
extern int madongseokDirection;

#define madongseokPos madongseok.Pos
#define madongseokPosPrev madongseok.PosPrev
#define madongseokAggro madongseok.Aggro
#define madongseokAggroPrev madongseok.AggroPrev
#define madongseokStamina madongseokStamina
#define madongseok(mem) madongseok##mem

extern int p;

#endif //! GLOBAL_H