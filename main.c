#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

// 파라미터
#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0 // 마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5
// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0
// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

void print_train(int len, int C_pos, int Z_pos, int M_pos) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < len; j++) {
            if (i == 0 || i == 2) {
                printf("#");
                if (j == len - 1) {
                    printf("\n");
                }
            }
            else {
                if (j == 0) printf("#");
                else if (j == len - 1) printf("#\n");
                else if (j == C_pos) printf("C");
                else if (j == Z_pos) printf("Z");
                else if (j == M_pos) printf("M");
                else printf(" ");
            }
        }
    }
}

bool checkIsValidAggroRange(int aggro) {
    if (aggro > AGGRO_MIN && aggro < AGGRO_MAX) {
        return true;
    } else {
        return false;
    }
}

bool checkIsValidStaminaRange(int stamina) {
    if (stamina > STM_MIN && stamina < STM_MAX) {
        return true;
    } else {
        return false;
    }
}

int move_C(int C_pos, int p) {
    int random_num = rand() % 100;
    if (random_num < (100 - p)) {
        if (C_pos > 1) {
            C_pos--;
        }
    }
    return C_pos;
}

int move_Z(int p, int Z_pos ,int C_pos, int M_pos, int C_Aggro, int M_Aggro) {
    int random_num = rand() % 100;
    if (random_num > (100 - p)) {
        if (Z_pos > 1 && C_Aggro > M_Aggro) {
            if (Z_pos - 1 != C_pos) Z_pos--;
        } else if (Z_pos > 1 && C_Aggro < M_Aggro) {
            if (Z_pos + 1 != M_pos) Z_pos++;
        } else if (Z_pos > 1 && C_Aggro == M_Aggro) {
            if (Z_pos - 1 != C_pos) Z_pos--;
        }
    }
    return Z_pos;
}

int move_M(int M_pos, int Z_pos) {
    if (M_pos != Z_pos + 1) {
        M_pos--;
    } else {
        M_pos;
    }
    return M_pos;
}

int set_zombie_target(int Z_pos, int prev_Z_pos, int target) {
    if (Z_pos + 1 == prev_Z_pos) {
        target = ATK_CITIZEN;
    } else if (Z_pos - 1 == prev_Z_pos) {
        target = ATK_DONGSEOK;
    } else {
        target = ATK_NONE;
    }
    return target;
}

int main() {
    srand((unsigned int)time(NULL));
    int train = 0;
    int madongseok_stamina = 0;
    int p = 0;
    int C_pos = 0;
    int C_Aggro = 1;
    int Z_pos = 0;
    int M_pos = 0;
    int M_Aggro = 1;
    int target = ATK_NONE;
    bool isPulled = false;

    while (1) {
        printf("train length(%d ~ %d)>> ", LEN_MIN, LEN_MAX);
        scanf("%d", &train);
        if (train >= LEN_MIN && train <= LEN_MAX) break;
    }

    while (1) {
        printf("madongseok stamina(%d ~ %d)>> ", STM_MIN, STM_MAX);
        scanf("%d", &madongseok_stamina);
        if (madongseok_stamina >= STM_MIN && madongseok_stamina <= STM_MAX) break;
    }

    while(1) {
        printf("percentile probability 'p'(%d ~ %d)>> ", PROB_MIN, PROB_MAX);
        scanf("%d", &p);
        if (p >= PROB_MIN && p <= PROB_MAX) break;
    }
    C_pos = train - 6;
    Z_pos = train - 3;
    M_pos = train - 2;

    int prev_Z_pos = 0;
    int prev_M_pos = 0;
    int prev_M_Aggro = 0;
    int prev_C_pos = 0;
    int prev_C_Aggro = 0;
    int turn = 0;
    // 초기 열차 상태 출력.
    print_train(train, C_pos, Z_pos, M_pos);
    while (1) {
        turn++;
        prev_C_pos = C_pos;
        prev_C_Aggro = C_Aggro;
        prev_Z_pos = Z_pos;
        prev_M_pos = M_pos;
        prev_M_Aggro = M_Aggro;
        printf("\n\n");
        // 시민 움직임.
        C_pos = move_C(C_pos, p);
        if (prev_C_pos != C_pos) {
            if (checkIsValidAggroRange(C_Aggro)) C_Aggro++;
        } else {
            if (checkIsValidAggroRange(C_Aggro)) C_Aggro--;
        }
        // 좀비 움직임.
        if (turn % 2 == 0 && isPulled == false) Z_pos = move_Z(p, Z_pos, C_pos, M_pos, C_Aggro, M_Aggro);
        target = set_zombie_target(Z_pos, prev_Z_pos, target);
        isPulled = false;
        // 열차 상태 출력.
        print_train(train, C_pos, Z_pos, M_pos);
        printf("\n");
        // 시민 이동 로그 출력.
        if (prev_C_pos != C_pos) {
            printf("citizen %d -> %d, aggro(%d -> %d)\n", prev_C_pos, C_pos, prev_C_Aggro, C_Aggro);
        } else {
            printf("citizen stay: %d\n", C_pos);
        }
        // 좀비 이동 로그 출력.
        if (turn % 2 != 0) {
            printf("zombie (cannot move)\n");
        } else {
            if (prev_Z_pos != Z_pos) {
            printf("zombie %d -> %d\n", prev_Z_pos, Z_pos);
            } else {
                printf("zombie stay: %d\n", Z_pos);
            }
        }
        printf("\n");
        // 마동석 이동 입력받기.
        int input_move_madongseok = 0;
        bool moved = false;
        while(1) {
            if (Z_pos != M_pos - 1) {
                printf("madongseok move(0:stay, 1:left)>> ");
                scanf("%d", &input_move_madongseok);
                if (input_move_madongseok >= MOVE_STAY && input_move_madongseok <= MOVE_LEFT) {
                    switch(input_move_madongseok) {
                        case MOVE_STAY:
                            if (checkIsValidStaminaRange(madongseok_stamina)) madongseok_stamina++;
                            if (checkIsValidAggroRange(M_Aggro))M_Aggro--;
                            break;
                        case MOVE_LEFT:
                            M_pos = move_M(M_pos, Z_pos);
                            moved = true;
                            break;
                    }
                    break;
                }
            } else {
                printf("madongseok move(0:stay)>> ");
                scanf("%d", &input_move_madongseok);
                if (input_move_madongseok == MOVE_STAY) {
                    if (checkIsValidStaminaRange(madongseok_stamina))madongseok_stamina++;
                    if (checkIsValidAggroRange(M_Aggro))M_Aggro--;
                    break;
                }
            }
        }
        // 열차 상태 출력.
        print_train(train, C_pos, Z_pos, M_pos);
        printf("\n");
        // 마동석 이동 로그 출력.
        if (moved) {
            printf("madongseok: move %d -> %d(aggro: %d, stamina: %d)\n", prev_M_pos, M_pos, M_Aggro, madongseok_stamina);
        } else {
            printf("madongseok stay %d(aggro: %d, stamina: %d)\n", M_pos, M_Aggro, madongseok_stamina);
        }
        printf("\n");
        // 시민 행동.
        if (C_pos == 1) {
            printf("YOU WIN !!!!.");
            exit(0);
        } else {
            printf("citizen does nothing.\n");
        }
        // 좀비 행동.
        if (turn % 2 == 0 && isPulled == false) {
            switch(target) {
                case ATK_NONE:
                    printf("zombie attacked nobody\n");
                    break;
                case ATK_CITIZEN:
                    if (Z_pos - 1 == C_pos) {
                        printf("zombie attacked citizen.\n");
                        printf("GAME OVER citizen die.");
                        exit(0);
                    } else {
                        printf("zombie attacked nobody\n");
                    }
                case ATK_DONGSEOK:
                    if (Z_pos + 1 == M_pos) {
                        printf("zombie attakced madongseok.\n");
                        madongseok_stamina -= 1;
                        break;
                    }
                default:
                    break;
            }
        } else {
            printf("zombie attacked nobody.\n");
        }
        // 시민, 좀비 행동 로그 출력.
        // 마동석 행동 입력.
        int input_action = ACTION_REST;
        while (1) {
            if (Z_pos + 1 == M_pos) {
                printf("madongseok action(0.rest, 1.provoke, 2.pull)>>");
                scanf("%d", &input_action);
                if (input_action >= ACTION_REST && input_action <= ACTION_PULL) break;
            } else {
                printf("maddongseok action(0.rest, 1.provoke)>> ");
                scanf("%d", &input_action);
                if (input_action >= ACTION_REST && input_action <= ACTION_PROVOKE) break;
            }
        }
        int random_num = rand() % 100;
        // 마동석 행동 로그 출력.
        switch(input_action) {
            case ACTION_REST:
                printf("madongseok rest...\n");
                M_Aggro = (M_Aggro - 1 <= AGGRO_MIN) ? AGGRO_MIN : M_Aggro - 1;
                madongseok_stamina = (madongseok_stamina + 1 >= STM_MAX) ? STM_MAX : madongseok_stamina + 1;
                printf("madongseok: %d (aggro: %d, stamina: %d)\n", M_pos, M_Aggro, madongseok_stamina);
                break;
            case ACTION_PROVOKE:
                printf("madongseok provoked zombie...\n");
                M_Aggro = AGGRO_MAX;
                printf("madongseok: %d (aggro: %d, stamina: %d)\n", M_pos, M_Aggro, madongseok_stamina);
                break;
            case ACTION_PULL:
                if (random_num < (100 - p)) {
                    printf("madongseok pulled zombie... Next turn, it can`t move\n");
                    M_Aggro = (M_Aggro + 2 >= AGGRO_MAX) ? AGGRO_MAX : M_Aggro + 2;
                    madongseok_stamina -= 1;
                    isPulled = true;
                    printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", M_pos, prev_M_Aggro, M_Aggro, madongseok_stamina);
                    break;
                } else {
                    printf("madongseok failed to pull zombie...\n");
                    M_Aggro = (M_Aggro + 2 >= AGGRO_MAX) ? AGGRO_MAX : M_Aggro + 2;
                    madongseok_stamina -= 1;
                    printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", M_pos, prev_M_Aggro, M_Aggro, madongseok_stamina);
                    break;
                }
        }
        if (madongseok_stamina <= 0) {
            printf("GAME OVER! madongseok died.");
            exit(0);
        }
    }
    return 0;
}