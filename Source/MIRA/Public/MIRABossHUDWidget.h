// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Blueprint/UserWidget.h"
#include "MIRABossHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRABossHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindBossStat(class UMIRACharacterStatComponent* CharacterStat);
	void BindBossState(class AMIRAPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UTextBlock* BossName;

	UPROPERTY()
	class UTextBlock* BossAlias;
	
};
