// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "CoreMinimal.h"

// public macro settting for logging project MIRA
#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MIRA, Log, All);

// Add Logging Macros
#define MIRALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) +TEXT(")"))
#define MIRALOG_S(Verbosity) UE_LOG(MIRA, Verbosity, TEXT("%s"),*MIRALOG_CALLINFO)
#define MIRALOG(Verbosity, Format,...) UE_LOG(MIRA, Verbosity, TEXT("%s%s"),*MIRALOG_CALLINFO, *FString::Printf(Format,##__VA_ARGS__))
#define MIRACHECK(Expr, ...) {if(!(Expr)){MIRALOG(Error,TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
