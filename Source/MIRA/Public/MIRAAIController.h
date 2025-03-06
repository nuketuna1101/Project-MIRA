// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "AIController.h"
#include "MIRAAIController.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMIRAAIController();
	
	UFUNCTION(BlueprintCallable, Category = "AI Control")
	void SetAIAssets(UBlackboardData* NewBBAsset, UBehaviorTree* NewBTAsset);

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

#pragma region Declaration for BB Keys

	static const FName InitLocKey;
	static const FName TargetActorKey;
	static const FName TargetDistanceKey;

#pragma endregion

	void RunAI();
	void StopAI();

private:

#pragma region AI module : BB, BT

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
#pragma endregion

};
