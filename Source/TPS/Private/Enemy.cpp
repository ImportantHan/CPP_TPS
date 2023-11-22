#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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
	
	// 위젯 붙이기
	HPComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPComp"));
	HPComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FClassFinder<UUserWidget> TempHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/WBP_EnemyHP.WBP_EnemyHP_C'"));
	if ( TempHP.Succeeded() ) {
		HPComp->SetWidgetClass(TempHP.Class);
		HPComp->SetDrawSize(FVector2D(150, 20));
		HPComp->SetRelativeLocation(FVector(0, 0, 90));
		HPComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 캡슐과 메시의 충돌채널을 설정하고 싶다
	// 캡슐 - visible : block / camera : ignore
	// 메시 - visible : ignore / camera : ignore
	UCapsuleComponent* Cap = GetCapsuleComponent();
	auto mesh = GetMesh();
	Cap->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Cap->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);


}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector start = HPComp->GetComponentLocation();
	FVector target = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetCameraLocation();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

	HPComp->SetWorldRotation(NewRotation);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

