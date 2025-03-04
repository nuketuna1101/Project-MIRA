// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MIRA.h"
#include "GameFramework/GameModeBase.h"
#include "PoolComponent.h"
#include "MIRAGameMode.generated.h"

UCLASS(minimalapi)
class AMIRAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMIRAGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

#pragma region Pool Managing
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PoolManager")
	//UPoolComponent* BaseCharacterPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PoolManager")
	UPoolComponent* ProjectilePool;
#pragma endregion
};