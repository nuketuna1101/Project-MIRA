// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanTrooperCharacter.h"

AMarksmanTrooperCharacter::AMarksmanTrooperCharacter()
{
	// transform setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MARKSMANTROOPER(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (SK_MARKSMANTROOPER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MARKSMANTROOPER.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MARKSMANTROOPER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooperAnimBlueprint.MarksmanTrooperAnimBlueprint_C"));
	if (MARKSMANTROOPER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MARKSMANTROOPER_ANIM.Class);
	}
}
