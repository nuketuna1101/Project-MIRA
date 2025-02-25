// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacter.h"
#include "MIRAPlayerCharacter.generated.h"

/**
 * 
 */

/// <summary>
/// player character
/// 
/// unique properties
/// - camera components
/// - blade components
/// - input action mappings
/// </summary>
UCLASS()
class MIRA_API AMIRAPlayerCharacter : public AMIRABaseCharacter
{
	GENERATED_BODY()
	/*
public:
	AMIRAPlayerCharacter();


#pragma region Camera and Blade Components
	// components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* RightBlade;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* LeftBlade;

	// getter for weapon mesh
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeRight();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeLeft();

#pragma endregion
	*/
};
