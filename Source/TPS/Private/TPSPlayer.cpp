#include "TPSPlayer.h"
#include "Bullet.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EnemyFSM.h"

// 생성자
ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암을 생성해서 루트에 붙이고
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// 위치 X = 0, Y = 50, Z = 80으로 구현
	SpringArmComp->SetRelativeLocation(FVector(0, 50, 80));
	// 스프링암의 TargetArmlength값을 300으로
	SpringArmComp->TargetArmLength = 300;

	// 카메라를 생성해서 스프링암에 붙임
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Skeletal Mesh 적용
	// 생성자 도우미를 이용해서 Skeletal Mesh 로드(메시 주소 카피)
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	// 로드 성공 시 메시 적용
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	}

	// 캐릭터가 돌아감 - Z 축 기준
	bUseControllerRotationYaw = true;
	// 폰 중심 카메라 이동 체크박스
	SpringArmComp->bUsePawnControlRotation = true;
	// 이동하는 방향 바라보는 체크박스
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 캐릭터 컴포넌트에 있는 설정 변경
	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 0.7; // 공중 움직임

	// 유탄총 생성하고 에셋 적용 후 배치
	GrenadeGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeGun")); // 컴포넌트 등록
	GrenadeGun->SetupAttachment(GetMesh()); // Mesh 에 Attach
	GrenadeGun->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌처리 제외
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGrenadeGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'")); // 데이터로드
	if (TempGrenadeGunMesh.Succeeded()) { // 로드 성공시
		GrenadeGun->SetSkeletalMesh(TempGrenadeGunMesh.Object); // skeletal mesh 데이터 할당
		GrenadeGun->SetRelativeLocation(FVector(0, 40, 120)); // 위치 조정
	}

	// 스나이퍼 생성하고 Mesh 에 붙이기
	SniperGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGun"));
	SniperGun->SetupAttachment(GetMesh());
	SniperGun->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));
	if (TempSniperGunMesh.Succeeded()) {
		SniperGun->SetStaticMesh(TempSniperGunMesh.Object);
		SniperGun->SetRelativeLocation(FVector(0, 80, 130));
		SniperGun->SetRelativeScale3D(FVector(0.15f));
	}
}

// 비긴 플레이
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 처음 시작할 때 걷는 속도
	ActionWalk();

	// 태어날 때 두 개의 위젯 생성
	CrossHairUI = CreateWidget(GetWorld(), CrossHairUIFactory);
	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);
	// 다 넣어놓고 보였다 안보였다 하는 방식으로 구현
	CrossHairUI->AddToViewport(0);
	SniperUI->AddToViewport(0);
	
	// 부모에서 떼어내기
	// CrossHairUI->RemoveFromParent();

	// 처음 시작할 때 Grenade 만 보이도록 처리
	ATPSPlayer::ActionChooseGrenadeGun();

}

// 틱
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direction Normalize
	Direction.Normalize();
	// Direction 을 컨트롤러 기준으로 방향 재정렬
	FVector dir = FTransform(GetControlRotation()).TransformVector(Direction);
	// Direction 방향으로 이동
	AddMovementInput(dir);
	
	// cameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, 45, DeltaTime * 5);
}

// 입력값
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 함수들 모두 연결 (Project Setting 에 있는 이름과 연결)
	// 캐릭터 이동
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::AxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::AxisHorizontal);
	// 시야 이동
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::AxisTurn);
	// 점프, 총쏘기
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::ActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::ActionFire);
	// 유탄, 스나이퍼
	PlayerInputComponent->BindAction(TEXT("ChooseGrenadeGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseSniperGun);
	// 줌 인아웃
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATPSPlayer::ActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATPSPlayer::ActionZoomOut);

	// 걷기 뛰기
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ATPSPlayer::ActionRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ATPSPlayer::ActionWalk);
}

// 수직(앞뒤) 이동
void ATPSPlayer::AxisVertical(float value)
{
	Direction.X = value;
}
// 수평(좌우) 이동
void ATPSPlayer::AxisHorizontal(float value)
{
	Direction.Y = value;
}
// 상하 회전
void ATPSPlayer::AxisLookUp(float value)
{
	AddControllerPitchInput(value);
}
// 좌우 회전
void ATPSPlayer::AxisTurn(float value)
{
	AddControllerYawInput(value);
}
// 점프
void ATPSPlayer::ActionJump()
{
	Jump();
}

// 걷기 뛰기
void ATPSPlayer::ActionWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
}
void ATPSPlayer::ActionRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

// 유탄인지 체크해서 각각 상황에 맞는 총 발사 함수 호출
void ATPSPlayer::ActionFire()
{
	if ( GEngine ) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ActionFire"));

	if (bChooseGrenadeGun) {
		GrenadeFire();
	}
	else {
		SniperFire();
	}
}

// 유탄 발사
void ATPSPlayer::GrenadeFire()
{
	// 총알을 생성해서 유탄총의 총구 소켓 위치에 배치
	FTransform FirePosition = GrenadeGun->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, FirePosition);

}
// 스나이퍼 발사
void ATPSPlayer::SniperFire()
{
	// 앞방향 1Km 바라보기
	FHitResult HitResult;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 100000.f;
	FCollisionQueryParams Params;
	// 라인트레이스 : 바라보는 시선
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility)) {
		// 어딘가에 부딪히면 폭발 이펙트 표현
		FTransform position = FTransform(HitResult.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFXFactory, position);
		
		// 부딪힌 물체가 물리작용을 할 수 있다면 힘을 가함
		auto HitComp = HitResult.GetComponent();
		if ( HitComp && HitComp->IsSimulatingPhysics() ) {
			HitComp->AddForce(-HitResult.ImpactNormal * HitComp->GetMass() * 50000.f);
		}
		// 그 물체가 Enemy 라면 알려줌 (액터를 캐스트 해서 가져옴)
		AEnemy* Enemy = Cast<AEnemy>(HitResult.GetActor());
		if ( Enemy ) {
			// Enemy->EnemyFSM; // 아래 방법과 동일
			auto FSM = Enemy->GetDefaultSubobjectByName(TEXT("EnemyFSM"));
			UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(FSM);
			EnemyFSM->OnTakeDamage(1);
		}
	} else {
		// 부딪힌 곳이 없다(허공)
	}
}

// input 에서 1번 키 매핑 - 유탄총
void ATPSPlayer::ActionChooseGrenadeGun()
{
	bChooseGrenadeGun = true;

	GrenadeGun->SetVisibility(true);
	SniperGun->SetVisibility(false);
	CrossHairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);

}

// input 에서 2번 키 매핑 - 스나이퍼 총
void ATPSPlayer::ActionChooseSniperGun()
{
	bChooseGrenadeGun = false;

	GrenadeGun->SetVisibility(false);
	SniperGun->SetVisibility(true);
	CrossHairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

// 줌 In & Out (카메라 거리 조절)
// 줌 In 일 경우 Sniper 조준 보이게
// 줌 Out 일 경우 CrossHair 조준 보이게
void ATPSPlayer::ActionZoomIn()
{
	if (bChooseGrenadeGun) {
		return;
	}
	CameraComp->FieldOfView = 45;
	CrossHairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Visible);
}
void ATPSPlayer::ActionZoomOut()
{
	if (bChooseGrenadeGun) {
		return;
	}
	CameraComp->FieldOfView = 90;
	CrossHairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

