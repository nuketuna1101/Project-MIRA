// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Blueprint/UserWidget.h"
#include "MIRAHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMIRACharacterStatComponent* NewCharaterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateHPVisible();
	void InterpolatedHPUpdate();

private:
#pragma region HPBar Basic components
	TWeakObjectPtr<class UMIRACharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* PBHPBar;

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* TextActorName;
	
#pragma endregion

#pragma region HPBar display : basic variables and interp variables

	float CurrentDisplayedHP = 1.0f;

	FTimerHandle HPUpdateTimerHandle;
	float UpdateInterval = 0.02f;
	float InterpSpeed = 5.0f;

#pragma endregion
};
