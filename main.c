#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5

#define MOVE_LEFT 1
#define MOVE_STAY 0

#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int train[LEN_MAX];
int citizenPos[LEN_MAX/2];
int citizenAggro[LEN_MAX/2];
int numCitizen;
int zombiePos;
int zombieAggro;
int villainPos;
int villainAggro;
int madongseokPos;
int madongseokAggro;
int madongseokStamina;
int p;

int prevCitizenPos[LEN_MAX/2];
int prevZombiePos;
int prevMadongseokPos;

void savePrevPositions() {
  for (int i = 0; i < numCitizen; i++) {
    prevCitizenPos[i] = citizenPos[i];
  }
  prevZombiePos = zombiePos;
  prevMadongseokPos = madongseokPos;
}

void printMovement(int prevPos, int currentPos, int aggro, char* name) {
  printf("%s: ", name);
  if (prevPos != currentPos) {
    printf("%d -> %d (aggro: %d)\n", prevPos, currentPos, aggro);
  } else {
    printf("stay %d (aggro: %d)\n", currentPos, aggro);
  }
}

void initGame(int len, int stamina) {
  numCitizen = len / 4 + rand() % (len / 4);

  for (int i = 0; i < len; i++) train[i] = 0;

  citizenPos[0] = len - 1;
  citizenAggro[0] = 1;
  train[len-1] = 1;

  for (int i = 1; i < numCitizen; i++) {
    int pos = rand() % (len - 5) + 1;
    while (train[pos] != 0) pos = rand() % (len - 5) + 1;
    citizenPos[i] = pos;
    citizenAggro[i] = 1;
    train[pos] = 1;
  }

  zombiePos = len - 4; zombieAggro = 1; train[zombiePos] = 2;
  villainPos = len - 5; villainAggro = 1; train[villainPos] = 3;
  madongseokPos = len - 2; madongseokAggro = 1; madongseokStamina = stamina; train[madongseokPos] = 4;
}

void printTrain(int len) {
    printf("###");
    for (int i = 0; i < len; i++) printf("#");
    printf("\n");
    
    printf("# ");
    for (int i = 0; i < len; i++) {
        if (train[i] == 0) printf(" ");
        else if (train[i] == 1) printf("C");
        else if (train[i] == 2) printf("Z");
        else if (train[i] == 3) printf("V");
        else if (train[i] == 4) printf("M");
    }
    printf("#\n");
    
    printf("###");
    for (int i = 0; i < len; i++) printf("#");
    printf("\n");
}

void printStatus(int len) {
  for (int i = 0; i < numCitizen; i++) {
    if (citizenPos[i] >= 0) {
      printMovement(prevCitizenPos[i], citizenPos[i], citizenAggro[i], "citizen");
    }
  }
  if (zombiePos >= 0) {
    printMovement(prevZombiePos, zombiePos, zombieAggro, "zombie");
  }
  printf("madongseok: stay %d(aggro: %d, stamina: %d)\n", madongseokPos, madongseokAggro, madongseokStamina);
  printf("\n");
}

void moveCitizen() {
  for (int i = 0; i < numCitizen; i++) {
    int move = rand() % 100 < 100 - p;
    if (move && citizenPos[i] > 0) {
      train[citizenPos[i]] = 0;
      citizenPos[i]--;
      train[citizenPos[i]] = 1;
      citizenAggro[i] = (citizenAggro[i] + 1 <= AGGRO_MAX) ? citizenAggro[i] + 1 : AGGRO_MAX;
    } else {
      citizenAggro[i] = (citizenAggro[i] - 1 >= AGGRO_MIN) ? citizenAggro[i] - 1 : AGGRO_MIN;
    }
  }
  savePrevPositions();
}

void moveZombie(int len) {
  if (zombiePos >= 0) {
    int move = rand() % 2;
    if (move) {
      int newPos = zombiePos;
      if (madongseokAggro >= citizenAggro[0]) newPos++;
      else newPos--;

      if (newPos > 0 && newPos < len - 1 && newPos < madongseokPos && train[newPos] != 1 && train[newPos] != 3) {
        train[zombiePos] = 0;
        zombiePos = newPos;
        train[zombiePos] = 2;
      }
    }
  }
  savePrevPositions();
}

void moveVillain() {
  int move = citizenPos[0] - villainPos > 0;
  if (move) {
    train[villainPos] = 0;
    villainPos++;
    train[villainPos] = 3;
    villainAggro = (villainAggro + 1 <= AGGRO_MAX) ? villainAggro + 1 : AGGRO_MAX;
  } else {
    villainAggro = (villainAggro - 1 >= AGGRO_MIN) ? villainAggro - 1 : AGGRO_MIN;
  }
  savePrevPositions();
}

void moveMadongseok(int direction) {
  if (direction == MOVE_LEFT && madongseokPos > zombiePos + 1 && madongseokPos > 1) {
    train[madongseokPos] = 0;
    madongseokPos--;
    train[madongseokPos] = 4;
    madongseokAggro = (madongseokAggro + 1 <= AGGRO_MAX) ? madongseokAggro + 1 : AGGRO_MAX;
  } else {
    madongseokAggro = (madongseokAggro - 1 >= AGGRO_MIN) ? madongseokAggro - 1 : AGGRO_MIN;
  }
  savePrevPositions();
}

void citizenAction() {
    for (int i = 0; i < numCitizen; i++) {
        if (citizenPos[i] == 0) {
            train[citizenPos[i]] = 0;
            printf("citizen%d has escaped.\n", i);
            for (int j = i; j < numCitizen - 1; j++) {
                citizenPos[j] = citizenPos[j+1];
                citizenAggro[j] = citizenAggro[j+1];
            }
            numCitizen--;
            i--;
        }
    }
}

void villainAction() {
  if (villainPos == citizenPos[0] - 1 || villainPos == citizenPos[0] + 1) {
    if (rand() % 100 < 30) {
      int temp = citizenPos[0];
      citizenPos[0] = villainPos;
      villainPos = temp;
      temp = citizenAggro[0];
      citizenAggro[0] = villainAggro;
      villainAggro = temp;
      train[citizenPos[0]] = 1;
      train[villainPos] = 3;
    }
  }
}

void zombieAction() {
  int maxAggro = citizenAggro[0];
  int target = 0;
  for (int i = 1; i < numCitizen; i++) {
    if (citizenAggro[i] > maxAggro) {
      maxAggro = citizenAggro[i];
      target = i;
    }
  }
  if (villainAggro > maxAggro) {
    maxAggro = villainAggro;
    target = -1;
  }
  if (madongseokAggro > maxAggro) {
    maxAggro = madongseokAggro;
    target = -2;
  }

  if (zombiePos >= 0) {
    if (target >= 0 && (zombiePos == citizenPos[target] - 1 || zombiePos == citizenPos[target] + 1)) {
      printf("citizen%d has been attacked by zombie.\n", target);
      printf("%d citizen(s) alive.\n\n", numCitizen - 1);      
      train[citizenPos[target]] = 2;
      printf("citizen%d turned into a zombie!\n", target);
      printf("%d citizen(s) alive.\n\n", numCitizen - 1);      
      for (int j = target; j < numCitizen - 1; j++) {
        citizenPos[j] = citizenPos[j+1];
        citizenAggro[j] = citizenAggro[j+1];
      }
      numCitizen--;
    } else if (target == -1 && (zombiePos == villainPos - 1 || zombiePos == villainPos + 1)) {
        train[villainPos] = 2;
        villainPos = -1;
    } else if (target == -2 && (zombiePos == madongseokPos - 1 || zombiePos == madongseokPos + 1)) {
        madongseokStamina--;
    }
  }
}

void madonseokAction(int action) {
  if (action == ACTION_REST) {
    madongseokStamina = (madongseokStamina + 1 <= STM_MAX) ? madongseokStamina + 1 : STM_MAX;
    madongseokAggro = (madongseokAggro - 1 >= AGGRO_MIN) ? madongseokAggro - 1 : AGGRO_MIN;
  } else if (action == ACTION_PROVOKE) {
    madongseokAggro = AGGRO_MAX;
  } else if (action == ACTION_PULL && zombiePos == madongseokPos - 1) {
    madongseokAggro = (madongseokAggro + 2 <= AGGRO_MAX) ? madongseokAggro + 2 : AGGRO_MAX;
    madongseokStamina = (madongseokStamina - 1 >= STM_MIN) ? madongseokStamina - 1 : STM_MIN;
    if (rand() % 100 < 100 - p) zombiePos = -1;
  }
}

void printTab() {
  printf("\n\n");
}

int main() {
  int len, stamina;
  srand((unsigned int)time(NULL));

  while (1) {
    printf("train length(%d ~ %d)>> ", LEN_MIN, LEN_MAX);
    scanf("%d", &len);
    if (len >= LEN_MIN && len <= LEN_MAX) break;
  }

  while (1) {
    printf("madonseok stamina(%d ~ %d)>> ", STM_MIN, STM_MAX);
    scanf("%d", &stamina);
    if (stamina >= STM_MIN && stamina <= STM_MAX) break;
  }

  while (1) {
    printf("percentile probability 'p'(%d ~ %d)>> ", PROB_MIN, PROB_MAX);
    scanf("%d", &p);
    if (p >= PROB_MIN && p <= PROB_MAX) break;
  }

  printTab();
  initGame(len, stamina);
  printTrain(len);
  savePrevPositions();
  printTab();

  for (int stage = 1; stage <= 3; stage++) {
    printf("Stage %d start!\n", stage);

    while (1) {
      moveCitizen();
      if (stage >= 2) moveVillain();
      moveZombie(len);

      printTrain(len);
      printTab();
      printStatus(len);
      printTab();

      int dongseokMove;
      while (1) {
        printf("madonseok move(0:stay, 1:left)>> ");
        scanf("%d", &dongseokMove);
        if (dongseokMove == MOVE_STAY || dongseokMove == MOVE_LEFT) break;
      }
      moveMadonseok(dongseokMove);
      printTrain(len);

      citizenAction();
      if (stage >= 2) {
        villainAction();
        printf("villain ");
        if (train[villainPos] == 1) {
          printf("swapped position with citizen.\n");
        } else {
          printf("did nothing.\n");
        }
      } else {
        printf("citizen does nothing.\n");
      }

      int citizenAttacked = 0;
      for (int i = 0; i < numCitizen; i++) {
        if (train[citizenPos[i]] == 2) {
          citizenAttacked = 1;
           break;
        }
      }

      if (citizenAttacked) {
        printf("zombie attacked citizen.\n");
      } else if (train[madongseokPos] == 2) {
         printf("zombie attacked madonseok.\n");
      } else {
        printf("zombie attacked nobody.\n");
      }

      printTab();
            
      int dongseokAction;
      while (1) {
        printf("madonseok action(0.rest, 1.provoke, 2.pull)>> ");
        scanf("%d", &dongseokAction);
        if (dongseokAction == ACTION_REST || dongseokAction == ACTION_PROVOKE || dongseokAction == ACTION_PULL) break;
      }
      printTab();
      madonseokAction(dongseokAction);
      printStatus(len);

      if (numCitizen == 0) {
        printf("GAME OVER! All citizens are dead.\n");
        return 0;
      }

      int allEscaped = 1;
      for (int i = 0; i < numCitizen; i++) {
        if (citizenPos[i] != 0) {
           allEscaped = 0;
           break;
        }
      }
      if (allEscaped) break;

      if (madongseokStamina <= 0) {
        printf("GAME OVER! Madonseok is exhausted.\n");
        return 0;
      }
    }
  }
  printf("YOU WIN! All citizens escaped safely.\n");
  return 0;
}