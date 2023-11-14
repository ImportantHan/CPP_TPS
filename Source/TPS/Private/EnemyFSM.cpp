#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 Enemy 기억
	Me = Cast<AEnemy>(GetOwner());
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 상태에 따른 일 처리
	switch ( State )
	{
	case EEnemyState::Idle:
		TickIdle();
		break;
	case EEnemyState::Move:
		TickMove();
		break;
	case EEnemyState::Attack:
		TickAttack();
		break;
	case EEnemyState::Damage:
		TickDamage();
		break;
	case EEnemyState::Die:
		TickDie();
		break;
	}
}

void UEnemyFSM::TickIdle()
{
	// 플레이어 찾아 목적지로 기억
	Target = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// 찾은 후 이동 상태로 전이
	if ( Target ) {
		State = EEnemyState::Move;
	}
}

void UEnemyFSM::TickMove()
{
	// 목적지를 향해 이동
	FVector Dir = Target->GetActorLocation() - Me->GetActorLocation();
	// GetSafeNormal을 쓴 이유는 Normalize 로 인해 본래 길이가 변하지 않도록
	Me->AddMovementInput(Dir.GetSafeNormal());
	// 공격 가능 거리라면 공격 상태로 전이
	if ( Dir.Size() <= AttackDistance ) {
		State = EEnemyState::Attack;
	}
}

void UEnemyFSM::TickAttack()
{

}

void UEnemyFSM::TickDamage()
{
	// 2초동안 대기
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 2초 후 이동상태로 전이
	if ( CurrentTime > DamageTime ) {
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickDie()
{
	// 2초 동안 대기
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 파괴
	if ( CurrentTime > DieTime ) {
		Me->Destroy();
	}
	else {
		// 바닥으로 내려감
		// P = P0 + vt;
		/*FVector P0 = Me->GetActorLocation();
		FVector Velocity = FVector::DownVector * 200;
		FVector vt = Velocity * GetWorld()->GetDeltaSeconds();
		Me->SetActorLocation(P0 + vt);*/

		// 선형 보간 : Lerp (시작, 끝, 0.1) - t 고정일 경우 끝으로 가면서 서서히 느려짐
		// 시작 Me->GetActorLocation();
		// 끝 (기억하고 있어야 함) DieEndLoc : (Me->GetActorLocation() + FVector::DownVector * 200);
		// ds 는 보통 1/60 or 1/30
		// t : ds * 6
		FVector NewLoc = FMath::Lerp(Me->GetActorLocation(), DieEndLoc, GetWorld()->GetDeltaSeconds() * 2);
		Me->SetActorLocation(NewLoc);
	}
}

void UEnemyFSM::OnTakeDamage(int Damage)
{
	// 데미지 입었으면 체력 1감소
	HP -= Damage;
	// 체력이 0보다 크면 데미지 상태로 전이
	if ( HP > 0 ) {
		State = EEnemyState::Damage;
	}
	// 0 이하라면 죽음 상태로 전이
	else {
		State = EEnemyState::Die;
		// 바닥과 충돌하지 않게 충돌설정 제거
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DieEndLoc = Me->GetActorLocation() + FVector::DownVector * 200;
	}
}

void UEnemyFSM::SetState(EEnemyState Next)
{
	State = Next;
	CurrentTime = 0;
}

