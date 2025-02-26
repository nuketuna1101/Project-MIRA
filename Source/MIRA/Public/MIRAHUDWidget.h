// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABossHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "MIRAHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMIRACharacterStatComponent* CharacterStat);
	void BindPlayerState(class AMIRAPlayerState* PlayerState);

	void ShowBossHUD(bool bIsShowed);

protected:
	virtual void NativeConstruct()override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UMIRACharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AMIRAPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	UMIRABossHUDWidget* BossHUDWidget;
};
