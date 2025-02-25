// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Components/ActorComponent.h"
#include "MIRAPlayerCameraComponent.generated.h"

//UENUM()
//enum class ECameraMode : uint8
//{
//	FreeTPS,
//};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRA_API UMIRAPlayerCameraComponent : public UActorComponent
{
	GENERATED_BODY()
	/*
public:	
	// Sets default values for this component's properties
	UMIRAPlayerCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ECameraMode CurrentControlMode = ECameraMode::FreeTPS;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetCameraMode(ECameraMode CameraMode);

private:
#pragma region Camera Setting



	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	FRotator SpringArmRotation = FRotator::ZeroRotator;
	float SpringArmLength = 0.0f;
	float SpringArmRotationSpeed = 0.0f;
	float SpringArmLengthSpeed = 0.0f;
#pragma endregion
	*/
};
