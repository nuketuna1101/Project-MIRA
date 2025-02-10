# Troubleshooting Issue / Remarkable Issue

## Trooper AI Behavior Tree

1. Main Error : Trooper AI Behavior Tree

- Even though either BT service or decorator condition satisfied, "Move To" basic task did NOT work.

=> possible causes or solutions
1) NavMesh Issue
2) AIController does not include PathFollowingComponent

=> solved by NavMesh
: modify the NavMeshBoundsVolume large enough

<br>

2. Main Error : Trooper AI Behavior Tree **`Move To`** task

trooper ai가 플레이어를 감지하고 공격 범위에 진입하기 위해 추적 이동을 하고 공격으로 전환해야 하는데, 끝까지 도달하려고 움직인다.
그리고 move to 이동 속도 조절 필요

- Problem: **`Move To`** task does NOT change actor to other behavior until it reaches to target location.
- so as to quit moving if some condition satisfied, MUST create Abort condition.


3. Trooper의 공격 시점의 애님 몽타주가 재생되지 않음

로깅을 통해 확인하니 `MIRA: Error: UTrooperAnimInstance::UTrooperAnimInstance(18)AttackMontage is nullptr!`

에디터로 확인해보니 해당 트루퍼의 anim class는 플레이어가 사용하는 애님 인스턴스를 부모로 상속 받았음.

4. 제대로 c++ 코드 상 애님 몽타주를 할당했음에도 재생이 되지 않음

몽타주 재생 노드가 anim graph에 추가되어 있지 않아서 발생