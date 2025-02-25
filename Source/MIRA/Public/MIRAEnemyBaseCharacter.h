// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacterComponent.h"
#include "GameFramework/Character.h"
#include "MIRAEnemyBaseCharacter.generated.h"

// declare delegates
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegateTemp);

UCLASS()
class MIRA_API AMIRAEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMIRAEnemyBaseCharacter();

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
	FOnAttackEndDelegateTemp OnAttackEnd;

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


#pragma region Dead Timer

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	FTimerHandle DeadTimerHandle = {};
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AMIRACharacter* Target = nullptr;
};
