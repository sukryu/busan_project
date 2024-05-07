#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

int input_Train_Length() {
  int len_train = 0;
  while (1) {
    printf("train length(15 ~ 50)>> ");
    scanf("%d", &len_train);
    if (len_train >= LEN_MIN || len_train <= LEN_MAX) {
      return len_train;
    }
  }
  return len_train;
}

int input_Madongseok_Stamina() {
  int madong_Stamina = 0;
  while (1) {
    printf("madongseok stamina(0 ~ 5)>> ");
    scanf("%d", &madong_Stamina);
    if (madong_Stamina >= STM_MIN || madong_Stamina <= STM_MAX) {
      return madong_Stamina;
    }
  }
  return madong_Stamina;
}

int input_Percentile_Probability() {
  int percentile = 0;
  while (1) {
    printf("percentile probability 'p'(10 ~ 90)>> ");
    scanf("%d", &percentile);
    if (percentile >= PROB_MIN || percentile <= PROB_MAX) {
      return percentile;
    }
  }
  return percentile;
}

void print_Train(int train_length) {
  for (int i = 0; i < train_length; i++) {
    printf("#");
    if (i == train_length - 1) {
      printf("\n");
    }
  }
}

void print_Character(int train_length, int citizen, int zombie, int madongseok) {
  for (int i = 0; i < train_length; i++) {
    if (i == citizen) {
      printf("C");
    } else if (i == zombie) {
      printf("Z");
    } else if (i == madongseok) {
      printf("M");
    } else {
      printf(" ");
    }
    if (i == train_length - 1) {
      printf("\n");
    }
  }
}

void print_tab() {
  for (int i = 0; i < 2; i++) {
    printf("\n");
  }
}

int move_madong(int zombie_position, int madong_position, int madong_aggro) {
  int madong_move = 0;
  if (madong_position == zombie_position - 1) {
    printf("(0:stay)>> ");
    scanf("%d", &madong_move);
    if (madong_move != 0) {
      printf("invalid input\n");
      return 0;
    }
  }
  else {
    printf("(0:stay 1:left)>> ");
    scanf("%d", &madong_move);
    if (madong_move == 0) {
      if (madong_aggro >= AGGRO_MIN || madong_aggro <= AGGRO_MAX) {
        madong_aggro--;
      }
    }
    else if (madong_move == 1) {
      if (madong_aggro >= AGGRO_MIN || madong_aggro <= AGGRO_MAX) {
        madong_aggro++;
        madong_position--;
      }
    }
  }
  return madong_position, madong_aggro;
}

int main() {
  // 시드 설정.
  srand((unsigned int)time(NULL));
  int random_change = 0;
  int citizen_aggro = 0;
  int madong_aggro = 0;
  int count = 0;
  int madong_action = 0;
  int madong_move = 0;
  bool isActive_zombie = true;
  
  int train_length = 0;
  int madongseok_stamina = 0;
  int percentile_probability = 0;
  // 열차 길이, 마동석의 체력, 확률을 순차적으로 입력받음.
  train_length = input_Train_Length();
  madongseok_stamina = input_Madongseok_Stamina();
  percentile_probability = input_Percentile_Probability();

  // 띄어쓰기를 위해 3칸 띄워주는 함수.
  print_tab();

  int citizen = train_length - 6;
  int zombie = train_length - 3;
  int madongseok = train_length - 2;

  // 열차 출력.
  print_Train(train_length);
  // 각 캐릭터들의 위치 출력.
  print_Character(train_length, citizen, zombie, madongseok);
  print_Train(train_length);

  while (1) {
    // 확률을 구하고 시민부터 적용.
    random_change = rand() % 100 + 1;
    if (random_change <= percentile_probability && citizen > 1) {
      citizen--;
      citizen_aggro++;
    } else {
      citizen_aggro--;
    }

    // 좀비 이동.
    if (count % 2 != 0) {
      random_change = rand() % 100 + 1;
      if (random_change <= percentile_probability || zombie > 1 || zombie != citizen + 1 && isActive_zombie != false) {
        zombie--;
      }
    }
    
    // 마동석 이동.
    while (1) {
      if (zombie + 1 != madongseok) {
        printf("(0:stay, 1:left)>> ");
        scanf("%d", &madong_move);
        if (madong_move == 0) {
          madong_aggro--;
        } else if (madong_move == 1) {
          madong_aggro++;
          madongseok--;
        }
      }
    }
  }
  return 0;
}
