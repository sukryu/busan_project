#pragma once
#ifndef Busan_errno
#define Busan_errno unsigned char
typedef Busan_errno Busan_errno_t;
#endif // !Busan_errno

#ifndef OBJ_H
#define OBJ_H

typedef int obj_int;

typedef struct obj {
	char id;
	obj_int Pos, PosPrev, Aggro, AggroPrev;
}* ptr_obj;

enum err_obj {
    OBJ_GOOD,
    OBJ_OUT_OF_RANGE,
    OBJ_BLOCKED,
    OBJ_LMT
};

void obj_printMove(ptr_obj p);
Busan_errno_t obj_moveCond(ptr_obj p, char will);
Busan_errno_t obj_move(ptr_obj p, int will);
void obj_aggro(ptr_obj p, int will);

#endif // !OBJ_H
