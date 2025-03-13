// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MIRAGameState.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMIRAGameState();

public:
    int32 GetTotalGameScore() const { return TotalGameScore; }

    void AddGameScore() { TotalGameScore++; }

    void SetGameCleared();

    bool IsGameCleared()const;

private:

    UPROPERTY(Transient)
    int32 TotalGameScore;

    UPROPERTY(Transient)
    bool bGameCleared;


};
