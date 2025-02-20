// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Components/ActorComponent.h"
#include "MIRACharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHPDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

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

	// getter
	float GetPower();
	float GetHPRatio();
	int GetLevel() { return Level; }
	float GetHP() { return CurrentHP; }

#pragma region Delegates
	FOnZeroHPDelegate OnZeroHP;
	FOnHPChangedDelegate OnHPChanged;
#pragma endregion

private:
	struct FMIRACharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
