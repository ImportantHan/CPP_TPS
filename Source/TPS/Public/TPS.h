// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All)

#define HELLO_WORLD FString("Hello World!")

#define CLASS_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define MY_LOG(fmt, ...) UE_LOG(TPS, Warning, TEXT("%s %s"), *CLASS_INFO, *FString::Printf(fmt, ##__VA_ARGS__));