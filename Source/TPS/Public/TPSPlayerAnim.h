// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // 앞뒤로 움직이는 속력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction; // 좌우로 움직이는 속력
};
