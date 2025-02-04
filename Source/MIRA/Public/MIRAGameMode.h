// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MIRA.h"
#include "GameFramework/GameModeBase.h"
#include "MIRAGameMode.generated.h"

UCLASS(minimalapi)
class AMIRAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMIRAGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};



