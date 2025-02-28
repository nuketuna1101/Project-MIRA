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


5. 플레이어 캐릭터

공격 애니메이션 재생 중에 피격 당하면 피격 애니메이션 재생 후, 원래의 공격 애니메이션에서 애님 노티파이를 쏴줄 시점을 놓쳐서 인지 이후 로직이 엉망이 되는 현상 발생.

애니메이션에 관련하여 슬롯을 다르게 설정하도록 바꿀 생각


6. 플레이어의 투사체 발사

이때까지 적 npc의 투사체는 모두 플레이어 캐릭터에 대해 충돌 처리를 하는 방향으로 해결하면 간단했으나, 플레이어가 쏘는 투사체는 여러 가지 적 npc 클래스에 해당해야 함.
따라서 이에 대해 처리하는 정책은 여러가지 존재함.
ex. 모든 적npc 클래스를 하나하나 지정을 해주거나, 기본 MIRABaseCharacter를 타겟으로 지정하되 플레이어 캐릭터에 대해서는 제외해주기

따라서, 피격에 제외시켜주기 위해 
투사체 발사 시점에서 	Bullet->Owner = this; 로 투사체의 owner를 설정하고,
if (this->Owner->IsValidLowLevel() && HitActor == this->Owner.Get()) return; 으로 owner에 대해서는 충돌 탐지 제외시켜주었음.

