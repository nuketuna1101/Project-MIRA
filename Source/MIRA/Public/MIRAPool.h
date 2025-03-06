// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "MIRAPoolable.h"
#include "MIRAPool.generated.h"

UCLASS()
class MIRA_API AMIRAPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AMIRAPool();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "PoolManager")
	TArray<AMIRAPoolable*> ObjectPool;

public:
    template <typename T>
    void InitPool(const int32 Size);

	template <typename T>
	T* GetObject();

	template <typename T>
	void ReturnObject(T* Object);
};

template<typename T>
inline void AMIRAPool::InitPool(const int32 Size)
{
	UClass* ObjectClass = T::StaticClass();
	for (int32 i = 0; i < Size; ++i) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMIRAPoolable* NewActor = GetWorld()->SpawnActor<AMIRAPoolable>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		NewActor->Activate(false);
		ObjectPool.Add(NewActor);
	}
}

template<typename T>
inline T* AMIRAPool::GetObject()
{
	for (AMIRAPoolable* Object : ObjectPool)
	{
		if (!Object->GetIsActive())
		{
			Object->Activate(true);
			return Cast<T>(Object);
		}
	}

	UClass* ObjectClass = T::StaticClass();
	T* NewObject = GetWorld()->SpawnActor<T>(ObjectClass);
	ObjectPool.Add(NewObject);
	return NewObject;
}

template<typename T>
inline void AMIRAPool::ReturnObject(T* Object)
{
	Object->Activate(false);
}