// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMIRAUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MIRACHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	MIRACHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
