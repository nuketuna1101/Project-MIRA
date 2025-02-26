// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossIvan.h"
#include "MIRACharacterSetting.h"

AMIRABossIvan::AMIRABossIvan()
{
	// [DONT HAVE TO] Skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_IVAN(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant"));
	if (SK_IVAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_IVAN.Object);
	}

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
