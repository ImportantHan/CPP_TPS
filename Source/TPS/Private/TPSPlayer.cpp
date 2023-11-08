// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// ������
ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���������� �����ؼ� ��Ʈ�� ���̰�
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// ��ġ X = 0, Y = 50, Z = 80���� ����
	SpringArmComp->SetRelativeLocation(FVector(0, 50, 80));
	// ���������� TargetArmlength���� 300����
	SpringArmComp->TargetArmLength = 300;

	// ī�޶� �����ؼ� �������Ͽ� ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Skeletal Mesh ����
	// ������ ����̸� �̿��ؼ� Skeletal Mesh �ε�(�޽� �ּ� ī��)
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	// �ε� ���� �� �޽� ����
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	}

	// ĳ���Ͱ� ���ư� - Z �� ����
	bUseControllerRotationYaw = true;
	// �� �߽� ī�޶� �̵� üũ�ڽ�
	SpringArmComp->bUsePawnControlRotation = true;
	// �̵��Ѵ� ���� �ٶ󺸴� üũ�ڽ�
	GetCharacterMovement()->bOrientRotationToMovement = false; 

	// ĳ���� ������Ʈ�� �ִ� ���� ����
	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 0.7; // ���� ������
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direction Normalize
	Direction.Normalize();
	// Direction �� ��Ʈ�ѷ� �������� ���� ������
	FVector dir = FTransform(GetControlRotation()).TransformVector(Direction);
	// Direction �������� �̵�
	AddMovementInput(dir);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �Է� �Լ��� ��� ���� (Project Setting �� �ִ� �̸��� ����)
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::AxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::AxisHorizontal);

	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::AxisTurn);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::ActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::ActionFire);
}

void ATPSPlayer::AxisVertical(float value)
{
	Direction.X = value;
}

void ATPSPlayer::AxisHorizontal(float value)
{
	Direction.Y = value;
}

void ATPSPlayer::AxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::AxisTurn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::ActionJump()
{
	Jump();
}

void ATPSPlayer::ActionFire()
{
}

