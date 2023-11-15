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

	// 충돌체를 루트로
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp")); // 충돌체 등록
	SetRootComponent(CollisionComp); // 루트로 등록
	CollisionComp->SetSphereRadius(12.5f); // 충돌체 크기 설정
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll")); // 충돌 프로파일 설정

	// 외관을 충돌체에 붙이고
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp")); // 외관 컴포넌트 등록
	MeshComp->SetupAttachment(CollisionComp); // 부모 컴포넌트 지정
	MeshComp->SetRelativeScale3D(FVector(0.25f)); // 외관 크기 설정
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화

	// 이동컴포넌트 만들고 속력, 튕김 설정
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp")); // 발사체 컴포넌트
	MovementComp->InitialSpeed = 3000; // 초기속도
	MovementComp->MaxSpeed = 3000; // 최대속도
	MovementComp->bShouldBounce = true; // 반동여부(b : boolean)
	MovementComp->Bounciness = 0.3f; // 반동 값

	// 따라다닐 component 설정
	MovementComp->SetUpdatedComponent(CollisionComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	
	// 타이머 - 함수 사용 or 람다
	// GetWorld()->GetTimerManager().SetTimer(handle, this, &ABullet::DestroyBullet, 1);
	GetWorld()->GetTimerManager().SetTimer(handle, 
		FTimerDelegate::CreateLambda(
			[this]()->void {
				// TO DO 내용 작성, 람다를 사용해서 매번 함수를 만들어줘야하는 귀찮음 해결
				this->Destroy();
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("총알 사라짐"));*/
			}),
		1, false);

	int Sum = 0;
	// * 참고 - 람다식 작성 방법 *
	// [캡처](매개변수)->반환자료형{스코프, 구현}
	// []안에는 스코프에 넘겨줄 값 작성(주소값, call by value 가 되지 않게)
	// auto LambdaAddFunc = [&Sum](int32 a, int32 b) -> void { Sum = a + b; };
	// LambdaAddFunc(10, 20);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 타이머에 사용되는 함수
//void ABullet::DestroyBullet() {
//	Destroy();
//}