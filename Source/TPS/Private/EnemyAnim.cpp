// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::OnDamageEnd()
{
	// Enemy 를 가져와서 EnemyFSM 의 함수 호출
	AEnemy* Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( Enemy ) {
		Enemy->EnemyFSM->OnChangeMoveState();
	}
}

void UEnemyAnim::AnimNotify_DamageEnd()
{
	AEnemy* Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( Enemy ) {
		Enemy->EnemyFSM->OnChangeMoveState();
		Montage_Stop(0.1f, Enemy->EnemyFSM->EnemyActionMontage);
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	// 죽음 애니메이션이 종료되면 IsDieDone 을 true 로 바꿔서 바닥으로 내려가게
	IsDieDone = true;
}
