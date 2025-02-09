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
