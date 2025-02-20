// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MIRAUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeClicked();


protected:
	UPROPERTY()
	class UButton* ResumeButton;

};
