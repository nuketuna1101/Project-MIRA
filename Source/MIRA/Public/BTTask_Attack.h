// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    void Attack(AController* Controller);
    void StartStrafing(AController* Controller);

    FTimerHandle AttackTimerHandle;
    FTimerHandle StrafeTimerHandle;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackDelay = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float StrafeDuration = 3.0f;
};
