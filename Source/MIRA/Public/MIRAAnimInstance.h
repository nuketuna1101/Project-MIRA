// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Animation/AnimInstance.h"
#include "MIRAAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMIRAAnimInstance();

	// 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsInAir;
};
