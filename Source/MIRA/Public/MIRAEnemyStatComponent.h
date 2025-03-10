// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Components/ActorComponent.h"
#include "MIRAEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEnemyHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRA_API UMIRAEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIRAEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// setter
	void SetNewStat(int32 Index);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void SetPower(float NewPower);

	// getter
	float GetHPRatio();
	float GetHP() { return CurrentHP; }
	float GetPower();
	FString GetEnemyName();
	FString GetEnemyAlias();

#pragma region Delegates
	FOnEnemyHPZeroDelegate OnEnemyHPZero;
	FOnEnemyHPChangedDelegate OnEnemyHPChanged;
#pragma endregion

private:
	struct FMIRAEnemyCharacterData* CurrentEnemyData = nullptr;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Enemy Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Enemy Stat", Meta = (AllowPrivateAccess = true))
	float CurrentPower;
};
