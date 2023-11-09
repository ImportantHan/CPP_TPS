// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü�� ��Ʈ��
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp")); // �浹ü ���
	SetRootComponent(CollisionComp); // ��Ʈ�� ���
	CollisionComp->SetSphereRadius(12.5f); // �浹ü ũ�� ����
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll")); // �浹 �������� ����

	// �ܰ��� �浹ü�� ���̰�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp")); // �ܰ� ������Ʈ ���
	MeshComp->SetupAttachment(CollisionComp); // �θ� ������Ʈ ����
	MeshComp->SetRelativeScale3D(FVector(0.25f)); // �ܰ� ũ�� ����
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ

	// �̵�������Ʈ ����� �ӷ�, ƨ�� ����
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp")); // �߻�ü ������Ʈ
	MovementComp->InitialSpeed = 3000; // �ʱ�ӵ�
	MovementComp->MaxSpeed = 3000; // �ִ�ӵ�
	MovementComp->bShouldBounce = true; // �ݵ�����(b : boolean)
	MovementComp->Bounciness = 0.3f; // �ݵ� ��

	// ����ٴ� component ����
	MovementComp->SetUpdatedComponent(CollisionComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

