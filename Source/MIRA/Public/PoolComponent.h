// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRAPool.h"
#include "Components/ActorComponent.h"
#include "PoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRA_API UPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;



public:
    AMIRAPool* ProjectilePool;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling Manager")
    TSubclassOf<AActor> ObjectClass;

    template <typename T>
    T* CreateObjectFromClass(UClass* ObjectClass)
    {
        return Cast<T>(GetWorld()->SpawnActor(ObjectClass));
    }


    /*
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling Manager")
    TSubclassOf<AActor> ObjectClass;

    int32 InitPoolSize = 3;

    int32 AddPoolSize = 1;

    UFUNCTION(BlueprintCallable, Category = "Pooling Manager")
    AActor* GetObject();

    UFUNCTION(BlueprintCallable, Category = "Pooling Manager")
    void ReturnObject(AActor* Object);
		
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    TArray<AActor*> ObjectPool;
    //TQueue<AActor*> ObjectPool;

    AActor* CreateObject();

    void ActivateObject(AActor* Object);

    void DeactivateObject(AActor* Object);
    */
};
