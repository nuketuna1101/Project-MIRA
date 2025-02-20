// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIRABaseCharacterComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnZeroHPDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRA_API UMIRABaseCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIRABaseCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// setter
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);

	// getter
	float GetPower();
	float GetHPRatio();
	float GetHP() { return CurrentHP; }

#pragma region Delegates
	FOnZeroHPDelegate OnZeroHP;
	FOnHPChangedDelegate OnHPChanged;
#pragma endregion

	
private:
	struct FMIRACharacterData* CurrentStatData = nullptr;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
