// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IvanFireHomings.h"
#include "MIRABossIvan.h"
#include "MIRAAIController.h"

UBTTask_IvanFireHomings::UBTTask_IvanFireHomings()
{
	NodeName = TEXT("IvanFireHomings");
}

EBTNodeResult::Type UBTTask_IvanFireHomings::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	MIRALOG(Warning, TEXT("bt IvanFireHomings [ExecuteTask]"));

	auto Boss = Cast<AMIRABossIvan>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)	return EBTNodeResult::Failed;

	Boss->FireHomings();

	return EBTNodeResult::Succeeded;
}

void UBTTask_IvanFireHomings::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_IvanFireHomings::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	MIRALOG(Warning, TEXT("End fire homings"));
}
