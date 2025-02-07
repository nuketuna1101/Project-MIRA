// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

UBTTask_Attack::UBTTask_Attack()
{
    // constructor
    Nodename = TEXT("Attack");


    isAbleToAttack = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // Super::ExecuteTask(OwnerComp, NodeMemory);
    // execute task

    return EBTNodeResult::InProgress;
}

void UBTTask_Attack::Attack(AController *Controller)
{
    // attack
}

void UBTTask_Attack::StartStrafing(AController *Controller)
{
    // after attack, troopers wait with strafing : keep aim focus on target, and strafing
}
