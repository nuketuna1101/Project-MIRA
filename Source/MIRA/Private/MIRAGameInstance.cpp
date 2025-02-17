// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAGameInstance.h"

UMIRAGameInstance::UMIRAGameInstance()
{
	FString DataPath = TEXT("");
	static ConstructorHelpers::FObjectFinder<UDataTable> 
		DT_MIRACHARACTER(*DataPath);
	MIRACHECK(DT_MIRACHARACTER.Succeeded());

	MIRACharacterDataTable = DT_MIRACHARACTER.Object;
	MIRACHECK(MIRACharacterDataTable->GetRowMap().Num() > 0);
}

void UMIRAGameInstance::Init()
{
	Super::Init();


}

FMIRACharacterData* UMIRAGameInstance::GetMIRACharacterData(int32 Level)
{
	return MIRACharacterDataTable->FindRow<FMIRACharacterData>(*FString::FromInt(Level), TEXT(""));
}