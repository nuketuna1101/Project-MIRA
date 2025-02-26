// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacter.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAMarksmanTrooper.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAMarksmanTrooper : public AMIRABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMIRAMarksmanTrooper();

	virtual void PostInitializeComponents() override;


	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	//	AController* EventInstigator, AActor* DamageCauser) override;

	// attack
	virtual void Attack() override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTarget(AMIRAPlayerCharacter* TargetActor);

	// anim montage
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ui widgets
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBar;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

protected:
	virtual void BeginPlay() override;

	class AMIRAPlayerCharacter* Target = nullptr;

	UClass* BulletClass;

};
