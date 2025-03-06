// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolComponent.h"
#include "MIRAProjectile.h"

// Sets default values for this component's properties
UPoolComponent::UPoolComponent()
{

}

void UPoolComponent::BeginPlay()
{
    ProjectilePool = GetWorld()->SpawnActor<AMIRAPool>();
    ProjectilePool->InitPool<AMIRAProjectile>(10);
}

/*
void UPoolComponent::BeginPlay()
{
	Super::BeginPlay();

    MIRALOG(Warning, TEXT("[init pool]"));
	// init pool
    for (int i = 0; i < InitPoolSize; ++i)
    {
        AActor* Object = CreateObject();
        ReturnObject(Object);
    }
}

AActor* UPoolComponent::GetObject()
{  
    // if pool enough, get activated
    if (ObjectPool.Num() > 0)
    {
        MIRALOG(Warning, TEXT("[GetObject] enough"));
        //AActor* Object = nullptr;
        //ObjectPool.Dequeue(Object);
        AActor* Object = ObjectPool.Pop();
        ActivateObject(Object);
        return Object;
    }
    else
    {
        // if not enough, create more additionals and get again
        for (int i = 0; i < AddPoolSize; ++i)
        {
            MIRALOG(Warning, TEXT("[GetObject] not enough so add"));
            AActor* Object = CreateObject();
            if (Object)
            {
                DeactivateObject(Object);
                //ObjectPool.Enqueue(Object);
                ObjectPool.Push(Object);
            }
        }
        return GetObject();
    }
}

void UPoolComponent::ReturnObject(AActor* Object)
{
    if (!IsValid(this))
    {
        MIRALOG(Error, TEXT("ReturnObject() 호출 시 PoolComponent 객체 무효"));
        return;
    }

    // return to pool as deactivated
    if (Object)
    {
        MIRALOG(Warning, TEXT("[ReturnObject]"));
        DeactivateObject(Object);
        //ObjectPool.Enqueue(Object);
        ObjectPool.Push(Object);
    }
    else
    {
        MIRALOG(Error, TEXT("[ReturnObject] failed"));
    }
}

AActor* UPoolComponent::CreateObject()
{
    if (ObjectClass)
    {
        MIRALOG(Warning, TEXT("[CreateObject]"));
        return GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
    }
    else
    {
        MIRALOG(Error, TEXT("[CreateObject] failed"));
        return nullptr;
    }
}

void UPoolComponent::ActivateObject(AActor* Object)
{
    if (Object)
    {
        MIRALOG(Warning, TEXT("[ActivateObject]"));
        Object->SetActorHiddenInGame(false);
        Object->SetActorTickEnabled(true);
    }
}

void UPoolComponent::DeactivateObject(AActor* Object)
{
    if (Object)
    {
        MIRALOG(Warning, TEXT("[DeactivateObject]"));
        Object->SetActorHiddenInGame(true);
        Object->SetActorTickEnabled(false);
    }
}
*/