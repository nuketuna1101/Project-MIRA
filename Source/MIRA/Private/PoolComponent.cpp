// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolComponent.h"

// Sets default values for this component's properties
UPoolComponent::UPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// init pool
    for (int i = 0; i < InitPoolSize; ++i)
    {
        AActor* Object = CreateObject();
        if (Object)
        {
            DeactivateObject(Object);
            ObjectPool.Add(Object);
        }
    }
}

AActor* UPoolComponent::GetObject()
{
    // if pool enough, get activated
    if (ObjectPool.Num() > 0)
    {
        AActor* Object = ObjectPool.Pop();
        ActivateObject(Object);
        return Object;
    }
    else
    {
        // if not enough, create more additionals and get again
        for (int i = 0; i < AddPoolSize; ++i)
        {
            AActor* Object = CreateObject();
            if (Object)
            {
                DeactivateObject(Object);
                ObjectPool.Add(Object);
            }
        }
        return GetObject();
    }
}

void UPoolComponent::ReturnObject(AActor* Object)
{
    // return to pool as deactivated
    if (Object)
    {
        DeactivateObject(Object);
        ObjectPool.Add(Object);
    }
}

AActor* UPoolComponent::CreateObject()
{
    if (ObjectClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = GetOwner()->GetInstigator();

        return GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    }
    return nullptr;
}

void UPoolComponent::ActivateObject(AActor* Object)
{
    if (Object)
    {
        Object->SetActorHiddenInGame(false);
        Object->SetActorTickEnabled(true);
    }
}

void UPoolComponent::DeactivateObject(AActor* Object)
{
    if (Object)
    {
        Object->SetActorHiddenInGame(true);
        Object->SetActorTickEnabled(false);
    }
}