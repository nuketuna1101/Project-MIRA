// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossIvan.h"
#include "MIRACharacterSetting.h"

AMIRABossIvan::AMIRABossIvan()
{
}

void AMIRABossIvan::BeginPlay()
{
	Super::BeginPlay();

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->BossAssets[0];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));
}
