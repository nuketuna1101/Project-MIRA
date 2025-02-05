# Project MIRA

`Mirage In Ruined Age`

<br>

### Genre

3D Open world action
(Motive: Cyberpunk 2077, Assassin's Creed: Odessey, Red Dead Redemption 2)

<br>

### Concept

- Cyberpunk Universe
- You are an unfamouse fixer.
- You should take on some dangerous requests from unknown clients.

<br>

### Missions

> mission 1 : Operation Shadow
> 
> - assassinate the target
> - you would rather fix as quietly as possible without being exposed to guards
> - you must survive
> 
> 
> mission 2 : Operation Hammering
> 
> - eliminate all the enemies
> - enter the room and eliminate the boss
> - you must survive

<br><br><br>

# Details

## 1) Player Character



### 시스템 구성

#### 캐릭터 구성

1) 플레이어 캐릭터

단일 플레이어 캐릭터

(단, 확장 가능하도록 코드 설계)


2) 적 캐릭터

- 라이플 보병
(에셋: paragon wraith)

- 근접 공격 보병

- 미션1 타겟 (비무장)

- 미션2 보스


#### 주요 시스템 구현 과제

1) 플레이어 상호작용

- 기본 움직임
- 스킬의 사용
- 회피
- 

2) AI 패턴

- 일반 적 캐릭터의 AI 패턴 로직
순찰, 감지 시 추적
- 미션1 타겟의 AI 패턴 로직
Idle, 플레이어 발견 시 도망 등
- 미션2 보스의 보스 패턴
페이즈 1, 페이즈 2

3) UI

- 게임 시작 화면
- 인게임 내 HUD
- 메뉴
- 스킬/강화 화면


4) 타격감의 구현

- 추후 많이 고민할 부분

5) 시네마틱



- 보스방 입장시,

- 보스 페이즈 전환시

- 보스 처치 시




# input system

mouse left btn : basic attack

mouse right btn : forward view lock on

+ 벽 타기 추가하기

# player character skill system

1: dash     : Melee_Fail
2: 범위 공격 : 3개 정도의 검기 날리기 : 캐스팅 애니메이션 Attack_Melee_C_Dash