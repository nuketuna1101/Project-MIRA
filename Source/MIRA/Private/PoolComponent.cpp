// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolComponent.h"
#include "PoolableInterface.h"

// Sets default values for this component's properties
UPoolComponent::UPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// init pool
    CurPoolSize = 0;
    for (int i = 0; i < InitPoolSize; ++i)
    {
        AActor* Object = CreateObject();
        if (Object)
        {
            DeactivateObject(Object);
            ObjectPool.Enqueue(Object);
            CurPoolSize++;
        }
    }
}

AActor* UPoolComponent::GetObject()
{
    // if pool enough, get activated
    if (CurPoolSize > 0)
    {
        //AActor* Object = ObjectPool.Pop();
        //ActivateObject(Object);
        //return Object;

        AActor* Object;
        ObjectPool.Dequeue(Object);
        CurPoolSize--;
        if (IsValid(Object))
        {
            MIRALOG(Log, TEXT("GetObject: Object found in pool, Object = %p"), Object); 
            ActivateObject(Object);
            return Object;
        }
        else
        {
            MIRALOG(Error, TEXT("GetObject: Invalid object in pool!"));
            return nullptr;
        }
    }
    else
    {
        // if not enough, create more additionals and get again
        MIRALOG(Warning, TEXT("GetObject: Pool is empty, creating additional objects"));

        for (int i = 0; i < AddPoolSize; ++i)
        {
            AActor* Object = CreateObject();
            if (Object)
            {
                DeactivateObject(Object);
                ObjectPool.Enqueue(Object);
                CurPoolSize++;
            }
        }
        return GetObject();
    }
}

void UPoolComponent::ReturnObject(AActor* Object)
{
    // return to pool as deactivated
    //if (Object)
    //{
    //    DeactivateObject(Object);
    //    ObjectPool.Add(Object);
    //}

    if (IsValid(Object))
    {
        MIRALOG(Warning, TEXT("ReturnObject: Returning object to pool, Object = %p"), Object); // 오브젝트 주소 확인
        DeactivateObject(Object);
        ObjectPool.Enqueue(Object);
        CurPoolSize++;
        MIRALOG(Warning, TEXT("ReturnObject: Pool size = %d"), CurPoolSize);
    }
    else
    {
        MIRALOG(Error, TEXT("ReturnObject: Invalid object to return!"));
    }
}

AActor* UPoolComponent::CreateObject()
{
    if (ObjectClass)
    {
        MIRALOG(Warning, TEXT("씨발 생성"));
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = GetOwner()->GetInstigator();

        return GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    }
    MIRALOG(Warning, TEXT("씨발 생성 못함"));

    return nullptr;
}

void UPoolComponent::ActivateObject(AActor* Object)
{
    if (nullptr != Object)
    {
        Object->SetActorHiddenInGame(false);
        Object->SetActorTickEnabled(true);

        //IPoolableInterface* Poolable = Cast<IPoolableInterface>(Object);
        //if (Poolable)
        //{
        //    Poolable->Activate();
        //}
        //else
        //{
        //    MIRALOG(Warning, TEXT("ActivateObject: Object does not implement IPoolableInterface!"));
        //}
    }
    else
    {
        MIRALOG(Error, TEXT("ActivateObject: Invalid Object pointer!"));
    }
}

void UPoolComponent::DeactivateObject(AActor* Object)
{
    if (nullptr != Object)
    {


        //IPoolableInterface* Poolable = Cast<IPoolableInterface>(Object);
        //if (Poolable)
        //{
        //    Poolable->Deactivate();
        //}
        //else
        //{
        //    MIRALOG(Warning, TEXT("ActivateObject: Object does not implement IPoolableInterface!"));
        //}

        Object->SetActorHiddenInGame(true);
        Object->SetActorTickEnabled(false);
    }
    else
    {
        MIRALOG(Error, TEXT("ActivateObject: Invalid Object pointer!"));
    }
}