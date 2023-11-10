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

	// ��ź��
	UPROPERTY(EditAnywhere);
	class USkeletalMeshComponent* GrenadeGun;

	// ��������
	UPROPERTY(EditAnywhere);
	class UStaticMeshComponent* SniperGun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS);
	FVector Direction;

	UPROPERTY(EditAnywhere);
	TSubclassOf<class ABullet> BulletFactory;

	// �Է¿� �����ϴ� �Լ� ����
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);
	void ActionJump();

	// �ѽ�� ������ 
	void ActionFire();
	void GrenadeFire();
	void SniperFire();

	void ActionChooseGrenadeGun();
	void ActionChooseSniperGun();

	void ActionZoomIn();
	void ActionZoomOut();

	
	// ���� UI ���� - ũ�ν����, �������� ���� ����
	UPROPERTY(EditAnywhere);
	TSubclassOf<class UUserWidget> CrossHairUIFactory;
	
	UPROPERTY();
	class UUserWidget* CrossHairUI;

	UPROPERTY(EditAnywhere);
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY();
	class UUserWidget* SniperUI;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS);
	bool bChooseGrenadeGun;

	// VFXFactory �߰�
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFXFactory;
};
