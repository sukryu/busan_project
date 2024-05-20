#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// 열차 길이.
#define LEN_MIN 15
#define LEN_MAX 50

// 마동석 체력
#define STM_MIN 0
#define STM_MAX 5

// 확률
#define PROB_MIN 10
#define PROB_MAX 90

// 어그로 범위
#define AGGRO_MIN 0
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0 // 휴식
#define ACTION_PROVOKE 1 // 도발
#define ACTION_PULL 2

int get_TrainLength() {
  int result = 0;
  while(1) {
    printf("train length(%d ~ %d)>> ", LEN_MIN, LEN_MAX);
    scanf("%d", &result);
    if (result >= LEN_MIN && result <= LEN_MAX) {
      break;
    }
  }
  return result;
}

int get_MaSTM() {
  int result = 0;
  while (1) {
    printf("madonseok stamina(%d ~ %d)>> ", STM_MIN, STM_MAX);
    scanf("%d", &result);
    if (result >= STM_MIN && result <= STM_MAX) {
      break;
    }
  }
  return result;
}

int get_PercentileProbability() {
  int result = 0;
  while (1) {
    printf("percentile probability(%d ~ %d)>> ", PROB_MIN, PROB_MAX);
    scanf("%d", &result);
    if (result >= PROB_MIN && result <= PROB_MAX) {
      break;
    }
  }
  return result;
}

void print_train2(int length) {
  for (int i = 0; i < length; i++) {
    if (i == length - 1) printf("#\n");
    else printf("#");
  }
}
void print_train(int length, int C_po, int Z_po, int M_po) {
  print_train2(length);
  for (int i = 0; i < length; i++) {
    if (i == 0) printf("#");
    else if (i == length - 1) {
      printf("#\n");
    }
    else if (i == C_po) printf("C");
    else if (i == Z_po) printf("Z");
    else if (i == M_po) printf("M");
    else printf(" ");
  }
  print_train2(length);
}

void print_Tab() {
  for (int i = 0; i < 2; i++) {
    printf("\n");
  }
}

int move_Citizen(int rc, int pb, int cp) {
  if (rc <= pb && cp > 1) {
    cp--;
  }
  return cp;
}

int checkIsMoved(int before_Cp, int current_Cp) {
  if (current_Cp == (before_Cp - 1)) {
    return 1;
  } else {
    return 0;
  }
}

bool checkIsValidRangeOfAggro(int before_Aggro, int current_Aggro) {
  if (
    before_Aggro >= AGGRO_MIN && before_Aggro < AGGRO_MAX &&
    current_Aggro >= AGGRO_MIN && before_Aggro < AGGRO_MAX) {
      return true; 
    }
  return false;
}

int move_Zombie(int turn, int rc, int pb, int zp, int ma_ActionType, int cp_a, int m_a) {
  if (turn % 2 == 1) {
    if (rc <= pb && zp > 1) {
      if (ma_ActionType != ACTION_PULL) {
        if (cp_a >= m_a) zp--;
        else zp++;
      }
    }
  }
  return zp;
}

void trace_log(
  int before_cp, 
  int current_cp,
  int before_cp_aggro, 
  int current_cp_aggro,
  int before_zp,
  int current_zp) {
    printf("citizen: %d -> %d (aggro: %d -> %d)\n", before_cp, current_cp, before_cp_aggro ,current_cp_aggro);
    if (before_zp == current_zp) {
      printf("zombie: stay %d\n", current_zp);
    } else {
      printf("zombie: %d -> %d\n", before_zp, current_zp);
    }
}

int move_Madongseok(int actionType, int zp, int mp) {
  while (1) {
      if (zp == (mp - 1)) {
      printf("madonseok move(%d:stay)>> ", ACTION_REST);
      scanf("%d", &actionType);
      if (actionType == ACTION_REST) {
        break;
      }
    } else {
      printf("madonseok move(%d:stay, %d:left)>> ", ACTION_REST, ACTION_PROVOKE);
      scanf("%d", &actionType);
      if (actionType == ACTION_REST) break;
      else if (actionType == ACTION_PROVOKE) {
        mp--;
        break;
      }
    }
  }
  return mp;
}

void checkMadongseokIsMovedAndPrintTraceLog(
  int before_mp, 
  int current_mp,
  int mp_aggro,
  int mp_stamina
  ) {
  if (current_mp == (before_mp - 1)) {
    printf("madonseok: stay %d(aggro: %d, stamina: %d)\n", current_mp, mp_aggro, mp_stamina);
  } else {
    printf("madonseok rests...\n");
  }
}

int main() {
  srand((unsigned int)time(NULL));
  /*
  1. 열차 길이 입력받음. O
  2. 마동석 체력 입력 받음. O
  3. 확률 입력 받음. O
  4. 열차 상태 출력. O
  5. 시민, 좀비 순서대로 움직인 후 열차 출력. O
  6. 시민, 좀비 추적 로그 출력. O
  7. 마동석 이동 커맨드 입력(0, 1) O
  8. 7번을 토대로 다시 열차 출력. O
  9. 시민이 무엇을 했는지 출력 O
  10. 좀비가 무엇을 했는지 출력
  11. 마동석이 어떤 행동을 할 지 입력받음 (0, 1, 2)

  시뮬레이션이 끝날 때까지 무한 반복.
  */
  int train_length = 0;
  int ma_stm = 0;
  int pecentile_probability = 0;

  train_length = get_TrainLength();
  ma_stm = get_MaSTM();
  pecentile_probability = get_PercentileProbability();

  print_Tab();

  int citizen_position = train_length - 6;
  int zombie_potision = train_length - 3;
  int madonseok_position = train_length - 2;

  int citizen_Aggro = 0;
  int madonseok_Aggro = 0;
  int turn = 0;
  // 초기 열차 상태 출력.
  print_train(train_length, citizen_position, zombie_potision, madonseok_position);
  print_Tab();

  while (1) {
    turn++;
    // 확률 변수.
    int random_choice = 0;
    // 각 캐릭터의 이전 위치.
    int before_Citizen_Position = citizen_position;
    int before_Zombie_Position = zombie_potision;
    int before_Madonseok_Position = madonseok_position;

    // 시민과 마동석의 어그로 값.
    int before_Citizen_Aggro = citizen_Aggro;
    int before_Madonseok_Aggro = madonseok_Aggro;

    // 마동석 행동
    int MadongSeok_ActionType = 0;
    // 시민 움직임.
    random_choice = rand() % 100 + 1;
    citizen_position = move_Citizen(random_choice, pecentile_probability, citizen_position);
    if (checkIsMoved(before_Citizen_Position, citizen_position)) {
      if (checkIsValidRangeOfAggro(before_Citizen_Aggro, citizen_Aggro)) {
        citizen_Aggro++;
      }
    } else {
      citizen_Aggro--;
    }

    // 좀비 움직임.
    random_choice = rand() % 100 + 1;
    zombie_potision = move_Zombie(turn, random_choice, pecentile_probability, zombie_potision, MadongSeok_ActionType, citizen_Aggro, madonseok_Aggro);

    print_train(train_length, citizen_position, zombie_potision, madonseok_position);
    print_Tab();
    trace_log(before_Citizen_Position, citizen_position, before_Citizen_Aggro, citizen_Aggro, before_Zombie_Position, zombie_potision);
    print_Tab();

    madonseok_position = move_Madongseok(MadongSeok_ActionType, zombie_potision, madonseok_position);
    print_train(train_length, citizen_position, zombie_potision, madonseok_position);
    print_Tab();

    checkMadongseokIsMovedAndPrintTraceLog(before_Madonseok_Position, madonseok_position, madonseok_Aggro, ma_stm);
    sleep(1);
  }
  return 0;
}