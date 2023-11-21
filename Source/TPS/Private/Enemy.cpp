#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// EnemyFSM 컴포넌트 생성
	EnemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));

	// 외형 변경
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/ Script / Engine.SkeletalMesh'/Game/Models/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	// 로드 성공 시 메시 적용
	if ( TempMesh.Succeeded() ) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치, 회전, 크기 적용
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0, 0, -90), 
			FRotator(0, -90, 0)
		);
		GetMesh()->SetRelativeScale3D(
			FVector( 0.9 )
		);
	}

	// 애니메이션 붙이기 - _C붙여줘야함
	ConstructorHelpers::FClassFinder<UEnemyAnim> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_Enemy.ABP_Enemy_C'"));
	if ( TempAnim.Succeeded() ) {
		GetMesh()->SetAnimClass(TempAnim.Class);
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

