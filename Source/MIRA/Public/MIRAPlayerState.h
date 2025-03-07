// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/PlayerState.h"
#include "MIRAPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateUpdate);

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMIRAPlayerState();

    int32 GetGameScore() const { return GameScore;  }
    int32 GetCharacterLevel() const { return CharacterLevel; }

    void InitPlayerData();

    //void AddGameScore();
    //void SetGameCleared();

    //bool IsGameCleared() const;

    FOnPlayerStateUpdate OnPlayerStateUpdate;

private:
    UPROPERTY(Transient)
    int32 GameScore;

    UPROPERTY(Transient)
    int32 CharacterLevel;



};
