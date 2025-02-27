// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling Manager")
    TSubclassOf<AActor> ObjectClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling Manager")
    int32 InitPoolSize = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling Manager")
    int32 AddPoolSize = 10;

    UFUNCTION(BlueprintCallable, Category = "Pooling Manager")
    AActor* GetObject();

    UFUNCTION(BlueprintCallable, Category = "Pooling Manager")
    void ReturnObject(AActor* Object);
		
protected:
    TArray<AActor*> ObjectPool;

    AActor* CreateObject();

    void ActivateObject(AActor* Object);

    void DeactivateObject(AActor* Object);
};
