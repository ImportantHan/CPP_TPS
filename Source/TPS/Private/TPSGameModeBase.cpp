// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPS.h"

void ATPSGameModeBase::BeginPlay()
{
	// �⺻��
	UE_LOG(LogTemp, Warning, TEXT("Hello, %s"), *FString("mmm"));

	// shift + alt + s (assist ����) // Shift + ctrl + f (assist ���� ��) - ���ϴ� �Լ��� �ٷ� �̵�
	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CLASS_INFO, TEXT("AHSHDA"));

	MY_LOG(TEXT("%s %s"), TEXT("11"), TEXT("33"));
}
