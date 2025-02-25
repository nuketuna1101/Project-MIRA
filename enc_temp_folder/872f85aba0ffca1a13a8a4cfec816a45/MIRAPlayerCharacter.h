// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIRABaseCharacter.h"
#include "MIRAPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAPlayerCharacter : public AMIRABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMIRAPlayerCharacter();
	/*
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// damage framework
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

#pragma region CharacterState

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const { return ECharacterState(); }

#pragma endregion

#pragma region Actions
	void Attack();
	void StartBlock();
	void StopBlock();
	void StartAim();
	void StopAim();
	void StartDash();
#pragma endregion

#pragma region Delegates for BP event
	// delegate for when attack hit
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAttackEnd OnAttackEndBP;
	// delegate for when be hitted
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHitEvent OnHitBP;
	// delegate for when start dash
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStartDash OnStartDashBP;
#pragma endregion

#pragma region Character Stat

	UPROPERTY(VisibleAnywhere, Category = "Character Data")
	class UMIRACharacterStatComponent* CharacterStat;
#pragma endregion

protected:

private:
*/
};
