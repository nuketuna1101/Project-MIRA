// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAGameInstance.h"

UMIRAGameInstance::UMIRAGameInstance()
{
	FString DataPath = TEXT("/Game/MIRA/GameData/MIRACharacterData.MIRACharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> 
		DT_MIRACHARACTER(*DataPath);
	MIRACHECK(DT_MIRACHARACTER.Succeeded());

	MIRACharacterDataTable = DT_MIRACHARACTER.Object;
	MIRACHECK(MIRACharacterDataTable->GetRowMap().Num() > 0);
}

void UMIRAGameInstance::Init()
{
	Super::Init();

    PrintMIRACharacterDataAll();
}

FMIRACharacterData* UMIRAGameInstance::GetMIRACharacterData(int32 Level)
{
	FMIRACharacterData* CharacterData = MIRACharacterDataTable->FindRow<FMIRACharacterData>(FName(*FString::FromInt(Level)), TEXT(""));

	if (nullptr == CharacterData)
	{
		MIRALOG(Error, TEXT("Character data not found for Level: %d"), Level);
		return nullptr;
	}

	return CharacterData;
}

void UMIRAGameInstance::PrintMIRACharacterDataAll()
{
    if (MIRACharacterDataTable)
    {
        TMap<FName, uint8*> MIRARowMap = MIRACharacterDataTable->GetRowMap();
        MIRALOG(Warning, TEXT("[MIRA CharacterData CSV Check]"));
        for (auto& Pair : MIRARowMap)
        {
            FName RowName = Pair.Key;
            uint8* RowData = Pair.Value;

            FMIRACharacterData* Data = reinterpret_cast<FMIRACharacterData*>(RowData);

            if (Data)
            {
                MIRALOG(Warning, TEXT("Row Name: %s"), *RowName.ToString());
                MIRALOG(Warning, TEXT("Level: %d"), Data->Level);
                MIRALOG(Warning, TEXT("MaxHP: %f"), Data->MaxHP);
                MIRALOG(Warning, TEXT("MaxMP: %f"), Data->MaxMP);
                MIRALOG(Warning, TEXT("Power: %f"), Data->Power);
                MIRALOG(Warning, TEXT("--------------------"));
            }
            else
            {
                MIRALOG(Error, TEXT("Invalid data for Row Name: %s"), *RowName.ToString());
            }
        }
    }
    else
    {
        MIRALOG(Error, TEXT("MIRACharacterDataTable is not valid."));
    }
}
