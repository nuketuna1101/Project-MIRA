// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRACharacterStatComponent.h"
#include "MIRAGameInstance.h"

// Sets default values for this component's properties
UMIRACharacterStatComponent::UMIRACharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	Level = 1;
	MaxBullets = 3;
	CurrentBullets = MaxBullets;
}

// Called when the game starts
void UMIRACharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMIRACharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UMIRACharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MIRACHECK(nullptr != MIRAGameInstance);

	CurrentStatData = MIRAGameInstance->GetMIRACharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		CurrentHP = CurrentStatData->MaxHP;
		MaxBullets = 3;
		CurrentBullets = MaxBullets;
	}
	else
	{
		MIRALOG(Error, TEXT("Level (%d) data doesn't Exist"), NewLevel);
	}
}

void UMIRACharacterStatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UMIRACharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnZeroHP.Broadcast();
	}
}

void UMIRACharacterStatComponent::SetBullets(float NewAmount)
{
	CurrentBullets = NewAmount;
	OnBulletChanged.Broadcast();
	if (CurrentBullets <= 0)
	{
		OnBulletEmpty.Broadcast();
	}
}

void UMIRACharacterStatComponent::ConsumeBullets()
{
	CurrentBullets--;
	OnBulletChanged.Broadcast();
	if (CurrentBullets <= 0)
	{
		OnBulletEmpty.Broadcast();
	}

	if (!IsValid(GetWorld()))
	{
		MIRALOG(Error, TEXT("뭐지씨발"));
		return;
	}


	// bullet auto reload
	if (GetWorld()->GetTimerManager().IsTimerActive(BulletReloadTimer))
	{
		float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(BulletReloadTimer);
		GetWorld()->GetTimerManager().SetTimer(BulletReloadTimer, this, &UMIRACharacterStatComponent::AutoReloadSingleBullet, RemainingTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(BulletReloadTimer, this, &UMIRACharacterStatComponent::AutoReloadSingleBullet, BulletReloadDuration, false);
	}
}

void UMIRACharacterStatComponent::SetMaxBullets(float NewAmount)
{
	MaxBullets = NewAmount;
}

void UMIRACharacterStatComponent::AutoReloadSingleBullet()
{
	if (CurrentBullets < MaxBullets)
	{
		CurrentBullets++;
		OnBulletChanged.Broadcast();
	}
	// 타이머 핸들 초기화 (선택 사항)
	//GetWorld()->GetTimerManager().ClearTimer(BulletReloadTimer);
	// 추가 소모가 있을 경우 다시 타이머 설정
	if (CurrentBullets < MaxBullets)
	{
		GetWorld()->GetTimerManager().SetTimer(BulletReloadTimer, this, &UMIRACharacterStatComponent::AutoReloadSingleBullet, BulletReloadDuration, false);
	}
}

float UMIRACharacterStatComponent::GetPower()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Power;
}

float UMIRACharacterStatComponent::GetHPRatio()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return(CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}