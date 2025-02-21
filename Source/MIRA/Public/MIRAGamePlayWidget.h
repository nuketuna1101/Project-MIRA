// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Blueprint/UserWidget.h"
#include "MIRAGamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

#pragma region Button Click Events
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnControlInfoClicked();

	UFUNCTION()
	void OnReturenToTitleClicked();
#pragma endregion

#pragma region Buttons

	UPROPERTY()
	class UButton* ResumeButton;

	UPROPERTY()
	class UButton* ControlInfoButton;

	UPROPERTY()
	class UButton* ReturnToTitleButton;
#pragma endregion

	UPROPERTY()
	UUserWidget* ControlInfoWidget;
};
