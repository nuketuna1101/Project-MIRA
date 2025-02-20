// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacterComponent.h"
#include "GameFramework/Character.h"
#include "MIRAEnemyBaseCharacter.generated.h"

// declare delegates
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class MIRA_API AMIRAEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMIRAEnemyBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AMIRACharacter* Target = nullptr;

public:	
	// post initial
	virtual void PostInitializeComponents() override;

	// damage framework
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// ui widgets
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBar;

	// attack
	virtual void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTarget(AMIRACharacter* TargetActor);

	// anim montage
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	class UTrooperAnimInstance* TrooperAnim;

	UPROPERTY(VisibleAnywhere, Category = "Trooper Data")
	class UMIRABaseCharacterComponent* TrooperStat;

};
