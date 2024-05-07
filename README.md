## Parameters ##

``` c
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
```

# 부산행2에서의 요구사항 정리.

1. train_length, madongseok_stamina, pecentile_probability 입력받기.

- train_length -> 범위 (15 ~ 50)
- madongseok_stamina -> 범위 (0 ~ 5)
- pecentile_probability -> 범위 (10 ~ 90)

=> 위 3가지 변수를 입력받을 때 유효한 값이 입력될 때까지 다시 입력받기 == while 문으로 하면 됨.

2. 열차 출력 -> 그 전 부산행과 동일함.

3. 열차 출력

# 페이즈 1 (이동)

1. 시민
  -> (100 - p) 확률로 움직임.
  -> 이동했을 때 어그로 +1 가만히 있으면 -1
  -> 어그로 제한 범위를 초과하지 못함. 만약 최솟값과 최댓값일 때는 그대로 둠.

2. 좀비
  -> 2턴마다 이동 가능(이동 불가능 턴 표시)
  -> 이전 턴에 마동석이 "붙들기"를 성공 ? 이동 불가 : 이동
  -> 이동 가능한 턴에서 마동석의 어그로 수치와 시민의 어그로 수치 중 높은 쪽으로 이동.
  -> 만약 둘의 어그로 수치가 같다면 시민에게 이동. 

3. 마동석 이동 규칙(직접 입력)

  * 입력값 (0). (STAY) -> 어그로 -1
  * 입력값 (1). (LEFT) -> 어그로 +1
  -> 좀비와 이미 인접한 경우 STAY만 출력.

# 페이즈 2 (행동)

1. 시민
  -> 탈출구(왼쪽 끝 == 1)에 도달 == 탈출.
  -> 아웃트로 ("YOU WIN!...) 출력.
  -> 그 외의 경우 아무 행동 x

2. 좀비 행동
  * 인접한 인간을 공격 <- 가장 먼저 고려.
  if 한 시민(시민/마동석)에 인접했을 때 그 시민 공격.
  if 둘 다 인접 -> 어그로 높은 애 공격.
  if 시민 공격 == 게임 오버 || 마동석 공격 == 체력 - 1 -> If 체력 == STM_MIN -> 게임 오버

3. 마동석 행동

  1. 좀비와 인접하지 않은 경우(직접 입력. 선택지 2개)
    * ACTION_REST: 휴식 -> 어그로 -1, 체력 +1
    * ACTION_PROVO: 도발 -> 어그로 최대치
  
  2. 좀비와 인접한 경우 (직접 입력, 선택지 3개)
    * ACTION_REST: 위와 동일
    * ACTION_PROVO: 위와 동일
    * ACTION_PULL: 붙들기
      -> 어그로 +2, 체력 -1
      -> (100 - p)확률로 성공
      -> if 성공 -> 좀비 다음 턴에 이동 불가(표시 해줘야 함.)