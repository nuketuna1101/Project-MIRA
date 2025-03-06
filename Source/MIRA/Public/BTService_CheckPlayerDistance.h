// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UBTService_CheckPlayerDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayerDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
