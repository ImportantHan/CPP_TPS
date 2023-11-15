#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

UCLASS()
class TPS_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 플레이어 선언
	UPROPERTY()
	class ATPSPlayer* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // 앞뒤로 움직이는 속력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction; // 좌우로 움직이는 속력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFalling; // 떨어지는지 확인
};
