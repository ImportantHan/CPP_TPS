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
	int HP = 2;
	void OnTakeDamage(int Damage);
	
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
};
