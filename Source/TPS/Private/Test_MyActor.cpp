﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_MyActor.h"

// Sets default values
ATest_MyActor::ATest_MyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATest_MyActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATest_MyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("틱틱"));*/
}

