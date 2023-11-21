#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	// 오너를 가져와서 유효성 체크
	if ( Player == nullptr ) {
		auto Owner = TryGetPawnOwner();
		Player = Cast<ATPSPlayer>(Owner);
	}
	if ( Player == nullptr ) {
		return;
	}
	// Actor Forward Vector
	FVector Forward = Player->GetActorForwardVector();
	// Actor Right Vector
	FVector Right = Player->GetActorRightVector();
	// Velocity
	FVector Velocity = Player->GetVelocity();
	// Speed 변수에 값 대입
	// Direction 변수에 값 대입
	Speed = FVector::DotProduct(Forward, Velocity);
	Direction = FVector::DotProduct(Right, Velocity);

	UE_LOG(LogTemp, Warning, TEXT("Speed : %f, Direction : %f"), Speed, Direction);

	// 캐릭터 무브먼트 에서 IsFalling 가져와서 IsFalling 값 변경
	UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
	IsFalling = Movement->IsFalling();
}