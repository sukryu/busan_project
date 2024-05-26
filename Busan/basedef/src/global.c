#include <Busan/basedef/const.h>
#include <Busan/basedef/global.h>
#include <Busan/basedef/train.h>

char train[LEN_MAX + 1] = {' ', };
char stage;
int trainLen;
int turn;

struct obj citizen[LEN_MAX >> 1] = {TRAIN_C, };
int citizenNum;

struct obj zombie = { TRAIN_Z, };
struct obj villain = { TRAIN_V,  };
struct obj madongseok = { TRAIN_M, };

int madongseokDirection;
int madongseokStamina;
int madongseokStaminaPrev;

int p;