# Project MIRA

![](https://velog.velcdn.com/images/nuketuna/post/c0c91d6b-27b5-4fac-8380-a1f262956e74/image.png)

`Mirage In Ruined Age`

### Genre

3D Open world action
(Motive: Cyberpunk 2077, Assassin's Creed: Odessey, Red Dead Redemption 2)

### Concept

- Cyberpunk Universe
- You are an unfamouse fixer.
- You should take on some dangerous requests from unknown clients.

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

<br><br>

# Details

## 1) Player Character

### input system

<details>
    <summary> input system </summary>
    <div markdown="1">
    mouse left btn : basic attack
    mouse right btn : forward view lock on
    WASD : baisc movement
    space bar : jump
    'R' key : dodge by quick backward moving
    'F' key : interact
    'V' key : assassinate target / execute victim
    'G' key : skill DASH attack
    </div>
</details>




## 2) Enemies

### Rifle Trooper

- most basic trooper
- only ranged attack with his rifle
- asset: paragon wraith

### Assualt Trooper

- armored trooper stronger than rifle trooper
- melee attack

### Grumpy Joe

- unarmed target in mission 1
- if noticing your approach, he runs away with screaming

### Ivan, the Darkwater

- boss character in mission 2
- asset: paragon revenant

- phase 1: 


`additional`

### Guard Turret

- stationary turrect
- guarding his own area


## 3) Temporary Memos

check for the details => [Temporary Memos](./docs/tempmemos.md)

<br>

## 4) Troubleshooting Issue / Remarkable Issue

check for the details => [Troubleshooting Issue / Remarkable Issue](./docs/TroubleshootingAndErrors.md)


<!-- <details>
    <summary> Troubleshooting Issue / Remarkable Issue </summary>
    <div markdown="1">

    1. Main Error : Trooper AI Behavior Tree

    - Even though either BT service or decorator condition satisfied, "Move To" basic task did NOT work.

    => possible causes or solutions
    1) NavMesh Issue
    2) AIController does not include PathFollowingComponent

    

    </div>
</details> -->