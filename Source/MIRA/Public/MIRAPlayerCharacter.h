// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacter.h"
#include "MIRAPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerActionDelegate, uint8, ActionIndex);

/**
 * 
 */

/// <summary>
/// player character
/// 
/// unique properties
/// - camera components
/// - blade components
/// - input action mappings
/// </summary>
 
UENUM()
enum class ETempCameraMode : uint8
{
	FreeTPS,
};

UCLASS()
class MIRA_API AMIRAPlayerCharacter : public AMIRABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMIRAPlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



#pragma region [PlayerOnly] Camera Components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
#pragma endregion

#pragma region [PlayerOnly] Blade Components
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* RightBlade;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* LeftBlade;

	// getter for weapon mesh
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeRight() { return RightBlade; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeLeft() { return LeftBlade; }
#pragma endregion



#pragma region Action Delegate
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnPlayerActionDelegate OnPlayerAction;
#pragma endregion


#pragma region [TO DO] Actions
	virtual void Jump() override;
	virtual void Attack() override;
	void StartBlock();
	void StopBlock();
	void StartAim();
	void StopAim();
	void StartDash();
#pragma endregion



protected:
	virtual void BeginPlay() override;

#pragma region [PlayerOnly] Camera Setting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ETempCameraMode CurrentControlMode = ETempCameraMode::FreeTPS;
	void SetCameraMode(ETempCameraMode CameraMode);
	FRotator SpringArmRotation = FRotator::ZeroRotator;
	float SpringArmLength = 0.0f;
	float SpringArmRotationSpeed = 0.0f;
	float SpringArmLengthSpeed = 0.0f;
#pragma endregion


#pragma region [TO DO] Attack Logics
	// attack logics
	void AttackMelee();
	void AttackRange();
	void PerformAttackCombo();
	void SaveAttackCombo();
	void ResetAttackCombo();
	void AttackCheck();

	void ThrowRanged();
#pragma endregion

#pragma region [TO DO] Projectile
	// attack logics
	UClass* BulletClass;
#pragma endregion


private:

#pragma region [PlayerOnly] Input Action by axis and action mappings
	// basic movements by axis mapping
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	// player input actions by action mapping
	void Block();
	void Dodge();
	void Execute();
#pragma endregion


};
