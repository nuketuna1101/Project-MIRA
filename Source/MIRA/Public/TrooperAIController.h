// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "AIController.h"
#include "TrooperAIController.generated.h"

UENUM()
enum class ETrooperState : uint8
{
	PATROL,
	TRACE,
	HUNT,
	IDLE,
};

/**
 * 
 */
UCLASS()
class MIRA_API ATrooperAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATrooperAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;


	// ai module: BB and BT
	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
};
