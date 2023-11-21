#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnim.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 Enemy 기억
	Me = Cast<AEnemy>(GetOwner());

	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

	Ai = Cast<AAIController>(Me->GetController());
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
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickMove()
{
	FVector Destination = Target->GetActorLocation();
	// 목적지를 향해 이동
	FVector Dir = Destination - Me->GetActorLocation();

	Ai->MoveToLocation(Destination);
	// GetSafeNormal을 쓴 이유는 Normalize 로 인해 본래 길이가 변하지 않도록
	Me->AddMovementInput(Dir.GetSafeNormal());
	// 공격 가능 거리라면 공격 상태로 전이
	if ( Dir.Size() <= AttackDistance ) {
		SetState(EEnemyState::Attack);
		CurrentTime = AttackTime;
	}


}

void UEnemyFSM::TickAttack()
{
	// 시간이 흐르다가 (AttackWait)
	CurrentTime += GetWorld()->GetDeltaSeconds();
	
	// 공격 타격 시간이 되면
	if ( AttackTime < CurrentTime ) {

		EnemyAnim->IsAttack = true;

		CurrentTime = 0;

		float Dist = FVector::Dist(Target->GetActorLocation(), Me->GetActorLocation());
		// 거리가 AttackDistance를 초과한다면
		if ( AttackDistance < Dist ) {
			// 이동상태로 전이
			SetState(EEnemyState::Move);
			EnemyAnim->IsAttack = false;
		}
		else {
			// 공격
			if ( GEngine ) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("공격"));
		}
	}
}

void UEnemyFSM::TickDamage()
{
	//// 2초동안 대기
	//CurrentTime += GetWorld()->GetDeltaSeconds();
	//// 2초 후 이동상태로 전이
	//if ( DamageTime < CurrentTime ) {
	//	SetState(EEnemyState::Move);
	//}
}

void UEnemyFSM::TickDie()
{
	// Enemyanim->IsDieDone 이 false 라면 함수를 종료
	if ( EnemyAnim->IsDieDone == false ) {
		return;
	}

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
		SetState(EEnemyState::Damage);
		PlayMontageDamage();
	}
	// 0 이하라면 죽음 상태로 전이
	else {
		SetState(EEnemyState::Die);
		PlayMontaegDie();
		// 바닥과 충돌하지 않게 충돌설정 제거
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DieEndLoc = Me->GetActorLocation() + FVector::DownVector * 200;
	}
}

// CurrentTime 초기화
void UEnemyFSM::SetState(EEnemyState Next)
{
	check(EnemyAnim);

	State = Next;

	EnemyAnim->State = Next;

	CurrentTime = 0;
}

// 데미지 몽타주
void UEnemyFSM::PlayMontageDamage()
{
	FName SectionName = TEXT("Damage0");
	if ( FMath::RandBool() ) {
		SectionName = TEXT("Damage1");
	}
	EnemyAnim->PlayMontageDamage(SectionName);
}

// 죽음 몽타주
void UEnemyFSM::PlayMontaegDie()
{
	//Me->PlayAnimMontage(EnemyActionMontage, 1, TEXT("Die"));
	FName SectionName = TEXT("Die");
	EnemyAnim->PlayMontageDamage(SectionName);
}

void UEnemyFSM::OnChangeMoveState()
{
	SetState(EEnemyState::Move);
}
