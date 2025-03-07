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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	//	AController* EventInstigator, AActor* DamageCauser) override;

	// attack
	virtual void Attack() override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTarget(AMIRAPlayerCharacter* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Attack")

	void FireProjectile();

	// anim montage
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);



	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


#pragma region UI HPbar and CharacterState
	// ui widgets
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBar;

	void SetCharacterState(ECharacterState NewState) override;
#pragma endregion

protected:
	virtual void BeginPlay() override;

	class AMIRAPlayerCharacter* Target = nullptr;

	UClass* BulletClass;

#pragma region Anim Instance
	UPROPERTY()
	class UTrooperAnimInstance* MMTrooperAnim;
#pragma endregion

#pragma region EFX and SFX
	UParticleSystem* EFX_MuzzleFire;
	USoundBase* SFX_TrooperFire;
#pragma endregion

};
