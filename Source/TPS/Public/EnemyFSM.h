#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 열거형 - 가장 위의 값을 기준으로 1씩 증가
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle UMETA(DisplayName = "IDLE"),
	Move UMETA(DisplayName = "MOVE"),
	Attack UMETA(DisplayName = "ATTACK"),

	Damage UMETA(DisplayName = "DAMAGE"),
	Die UMETA(DisplayName = "DIE"),
};
//const int Idle = 0;
//const int Move = 1;
//const int Attack = 2;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState State;

	// 나 (Enemy)
	UPROPERTY()
	class AEnemy* Me;
	// 타겟 (Player)
	UPROPERTY()
	class ATPSPlayer* Target;

	// 공격 가능 거리
	float AttackDistance = 200;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	// 데미지 입었으면 체력 1감소
	// 체력이 0보다 크면 데미지 상태로 전이
	// 0 이하라면 죽음 상태로 전이
	int32 HP = 2;
	int32 MaxHP = 3;
	void OnTakeDamage(int32 Damage);
	
	float CurrentTime;

	UPROPERTY(EditAnywhere)
	float DamageTime = 2;
	
	UPROPERTY(EditAnywhere)
	float DieTime = 2;

	UPROPERTY(EditAnywhere)
	float AttackTime = 2;

	void SetState(EEnemyState param1);

	// 죽었을 때 내려갈 목적지 정하기
	FVector DieEndLoc;

	// 상태가 바뀔 때 애니메이션 상태 변화
	// 생성자에서 직접 ABP_Enemy를 로드 - Enemy.cpp에서 초기화
	UPROPERTY(EditAnywhere)
	class UEnemyAnim* EnemyAnim;

	// 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyActionMontage;

	void PlayMontageDamage();
	void PlayMontaegDie();

	void OnChangeMoveState();

	// AIController를 이용해서 길찾기
	UPROPERTY()
	class AAIController* Ai;

	FVector RandomLocation;

	bool UpdateRandomLocation(FVector Origin, float Radious, FVector& OutLocation);

private:
	void UpdateHP(int32 NewHP);
};
