// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IvanFireHomings.h"
#include "MIRABossIvan.h"

UBTTask_IvanFireHomings::UBTTask_IvanFireHomings()
{
	NodeName = TEXT("IvanFireHomings");

	HomingsCount = 5;
}

EBTNodeResult::Type UBTTask_IvanFireHomings::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	//auto Boss = Cast<AMIRABossIvan>(OwnerComp.GetAIOwner()->GetPawn());
	//if (nullptr == Boss)	return EBTNodeResult::Failed;

	MIRALOG(Warning, TEXT("bt enemy attack [ExecuteTask]"));


	return EBTNodeResult::InProgress;
}

void UBTTask_IvanFireHomings::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (HomingsCount == 0)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_IvanFireHomings::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	MIRALOG(Warning, TEXT("End fire homings"));
}
