#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 전방선언 - 헤더추가를 하지 않기 위해, 컴파일 시간 단축(Include What You Use)
	// 컴포넌트 스프링암, 카메라
	UPROPERTY(EditAnywhere);
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere);
	class UCameraComponent* CameraComp;

	// 유탄총
	UPROPERTY(EditAnywhere);
	class USkeletalMeshComponent* GrenadeGun;

	// 스나이퍼
	UPROPERTY(EditAnywhere);
	class UStaticMeshComponent* SniperGun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS);
	FVector Direction;

	UPROPERTY(EditAnywhere);
	TSubclassOf<class ABullet> BulletFactory;

	// 입력에 대응하는 함수 제작
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);
	void ActionJump();

	// 총쏘기 나눠서 
	void ActionFire();
	void GrenadeFire();
	void SniperFire();

	void ActionChooseGrenadeGun();
	void ActionChooseSniperGun();

	void ActionZoomIn();
	void ActionZoomOut();

	
	// 조준 UI 설정 - 크로스헤어, 스나이퍼 위젯 공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrossHairUIFactory;
	
	UPROPERTY();
	class UUserWidget* CrossHairUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY();
	class UUserWidget* SniperUI;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	bool bChooseGrenadeGun;

	// VFXFactory 추가
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFXFactory;
};
