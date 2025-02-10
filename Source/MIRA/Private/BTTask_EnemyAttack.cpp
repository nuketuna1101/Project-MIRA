// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttack.h"
#include "TrooperAIController.h"
#include "MIRAEnemyBaseCharacter.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	NodeName = TEXT("Trooper Attack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//
	auto Trooper = Cast<AMIRAEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return EBTNodeResult::Failed;

	Trooper->Attack();
	Trooper->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});
	IsAttacking = true;


	return EBTNodeResult::InProgress;
}

void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
