// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttack.h"
#include "MIRAMarksmanTrooper.h"
#include "MIRAAIController.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	NodeName = TEXT("Trooper Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//
	auto Trooper = Cast<AMIRAMarksmanTrooper>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return EBTNodeResult::Failed;

	MIRALOG(Warning, TEXT("bt enemy attack [ExecuteTask]"));

	Trooper->Attack();
	Trooper->IsAttacking = true;

	return EBTNodeResult::InProgress;
}

void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	auto Trooper = Cast<AMIRAMarksmanTrooper>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return;

	if (!Trooper->IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
