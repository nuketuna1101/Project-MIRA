// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Components/ActorComponent.h"
#include "MIRACharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHPDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnBulletChanged);
DECLARE_MULTICAST_DELEGATE(FOnBulletEmpty);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRA_API UMIRACharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIRACharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// setter
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void SetBullets(float NewAmount);
	void ConsumeBullets();
	void SetMaxBullets(float NewAmount);
	void AutoReloadSingleBullet();

	// getter
	float GetPower();
	float GetHPRatio();
	float GetHP() { return CurrentHP; }
	int32 GetLevel() { return Level; }
	int32 GetCurrentBullets() { return CurrentBullets; }
	int32 GetMaxBullets() { return MaxBullets; }

#pragma region Delegates
	FOnZeroHPDelegate OnZeroHP;
	FOnHPChangedDelegate OnHPChanged;

	FOnBulletChanged OnBulletChanged;
	FOnBulletEmpty OnBulletEmpty;
#pragma endregion

private:
	struct FMIRACharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = true))
	int32 CurrentBullets;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = true))
	int32 MaxBullets;


#pragma region Bullet reloading

	FTimerHandle BulletReloadTimer;
	float BulletReloadDuration = 10.0f;

#pragma endregion
};
