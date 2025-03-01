// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IvanFireHomings.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UBTTask_IvanFireHomings : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_IvanFireHomings();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	int32 HomingsCount;
};
