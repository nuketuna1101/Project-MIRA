// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/PlayerController.h"
#include "MIRAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMIRAPlayerController();
	
protected:
	virtual void BeginPlay() override;

public:
	// components inits after instance created
	virtual void PostInitializeComponents() override;
	// use pawn by possessing
	virtual void OnPossess(APawn* aPawn) override;

	class UMIRAHUDWidget* GetHUDWidget() const { return HUDWidget; }


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMIRAHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMIRAGamePlayWidget> GamePlayWidgetClass;

	virtual void SetupInputComponent() override;

private:
	void OnGamePause();

	UPROPERTY()
	class UMIRAHUDWidget* HUDWidget;
	UPROPERTY()
	class UMIRAGamePlayWidget* GamePlayWidget;
};
