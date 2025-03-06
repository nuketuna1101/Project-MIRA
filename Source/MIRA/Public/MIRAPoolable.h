// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "MIRAPoolable.generated.h"

/// <summary>
/// MIRAPoolable : Base class for MIRA actors to be pooled
/// </summary>

UCLASS()
class MIRA_API AMIRAPoolable : public AActor
{
	GENERATED_BODY()
	
public:	
	AMIRAPoolable();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsActive;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool bActivationFlag);

	bool GetIsActive() const { return bIsActive; }
};
