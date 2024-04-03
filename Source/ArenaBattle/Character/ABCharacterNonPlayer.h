// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"	//10-10 비동기 로드
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

UCLASS(config=ArenaBattle) //10-10 
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface	
{
	GENERATED_BODY()

public:
	AABCharacterNonPlayer();

protected:
	void SetDead() override;
	//10-10 config를 통해 데이터를 불러오겠다
	void NPCMeshLoadCompleted();
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;	//ini파일 이름과 동일한 이름
	TSharedPtr<FStreamableHandle> NPCMeshHandle;   //비동기 로드를 위한 구조체 선언

protected:
	//10-10
	virtual void PostInitializeComponents() override;

// AI Section
protected:
	//12-3
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	
	//12-7
	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
