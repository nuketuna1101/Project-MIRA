// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAGameInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UMIRAGameInstance::UMIRAGameInstance()
{
    // import CSV datatable
	FString DataPath1 = TEXT("/Game/MIRA/GameData/MIRACharacterData.MIRACharacterData");
    FString DataPath2 = TEXT("/Game/MIRA/GameData/MIRAEnemyCharacterData.MIRAEnemyCharacterData");

	static ConstructorHelpers::FObjectFinder<UDataTable> 
		DT_MIRACHARACTER(*DataPath1);
	MIRACHECK(DT_MIRACHARACTER.Succeeded());
	MIRACharacterDataTable = DT_MIRACHARACTER.Object;
	MIRACHECK(MIRACharacterDataTable->GetRowMap().Num() > 0);
    static ConstructorHelpers::FObjectFinder<UDataTable>
        DT_MIRAENEMYCHARACTER(*DataPath2);
    MIRACHECK(DT_MIRAENEMYCHARACTER.Succeeded());
    MIRAEnemyCharacterDataTable = DT_MIRAENEMYCHARACTER.Object;
    MIRACHECK(MIRAEnemyCharacterDataTable->GetRowMap().Num() > 0);
}

void UMIRAGameInstance::Init()
{
    Super::Init();

    // test: datatable
    PrintMIRACharacterDataAll();
    PrintMIRAEnemyCharacterDataAll();
}

FMIRACharacterData* UMIRAGameInstance::GetMIRACharacterData(int32 Level)
{
    if (MIRACharacterDataTable == nullptr)
    {
        MIRALOG(Error, TEXT("MIRACharacterDataTable is null!"));
        return nullptr;
    }

	FMIRACharacterData* CharacterData = MIRACharacterDataTable->FindRow<FMIRACharacterData>(*FString::FromInt(Level), TEXT(""));

	if (nullptr == CharacterData)
	{
		MIRALOG(Error, TEXT("Character data not found for Level: %d"), Level);
		return nullptr;
	}

	return CharacterData;
}

FMIRAEnemyCharacterData* UMIRAGameInstance::GetMIRAEnemyCharacterData(FString EnemyName)
{
    FMIRAEnemyCharacterData* EnemyCharacterData = MIRAEnemyCharacterDataTable->FindRow<FMIRAEnemyCharacterData>(FName(*EnemyName), TEXT(""));

    if (nullptr == EnemyCharacterData)
    {
        MIRALOG(Error, TEXT("EnemyCharacterData data not found for EnemyName"));
        return nullptr;
    }

    return EnemyCharacterData;
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

void UMIRAGameInstance::PrintMIRAEnemyCharacterDataAll()
{
    if (MIRAEnemyCharacterDataTable)
    {
        TMap<FName, uint8*> MIRARowMap = MIRAEnemyCharacterDataTable->GetRowMap();
        MIRALOG(Warning, TEXT("[MIRA MIRAEnemyCharacterDataTable CSV Check]"));
        for (auto& Pair : MIRARowMap)
        {
            FName RowName = Pair.Key;
            uint8* RowData = Pair.Value;

            FMIRAEnemyCharacterData* Data = reinterpret_cast<FMIRAEnemyCharacterData*>(RowData);

            if (Data)
            {
                MIRALOG(Warning, TEXT("Row Name: %s"), *RowName.ToString());
                MIRALOG(Warning, TEXT("EnemyName: %s"), *Data->EnemyName);
                MIRALOG(Warning, TEXT("Alias: %s"), *Data->Alias);
                MIRALOG(Warning, TEXT("MaxHP: %f"), Data->MaxHP);
                MIRALOG(Warning, TEXT("Power: %f"), Data->Power);
                MIRALOG(Warning, TEXT("IsBoss: %s"), Data->IsBoss ? TEXT("true") : TEXT("false"));
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
        MIRALOG(Error, TEXT("MIRAEnemyCharacterDataTable is not valid."));
    }
}
