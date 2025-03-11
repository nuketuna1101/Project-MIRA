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
	void UpdateCharacterHP();
	void UpdatePlayerState();
	void UpdatePlayerBullets();

private:
	TWeakObjectPtr<class UMIRACharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AMIRAPlayerState> CurrentPlayerState;

	const FLinearColor AvailableBulletColor = FLinearColor(1.0f, 0.15f, 0.3f, 1.0f);
	const FLinearColor EmptyBulletColor = FLinearColor(0.15f, 0.0f, 0.0f, 1.0f);

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

	UPROPERTY()
	TArray<class UImage*> BulletImages;
};
