// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// ���漱�� - ����߰��� ���� �ʱ� ����, ������ �ð� ����(Include What You Use)
	// ������Ʈ ��������, ī�޶�
	UPROPERTY(EditAnywhere);
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere);
	class UCameraComponent* CameraComp;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS);
	FVector Direction;

	// �Է¿� �����ϴ� �Լ� ����
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);
	void ActionJump();
	void ActionFire();
};