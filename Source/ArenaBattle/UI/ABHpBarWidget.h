// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//7-5
//#include "Blueprint/UserWidget.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"	//14-5
#include "ABHpBarWidget.generated.h"

UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);
	 
protected:
	//해당 함수가 호출되는 타이밍은 UI관련된 모든 기능들이 거의 초기화 완료 
	virtual void NativeConstruct() override;

public:
	//14-5 UpdateStat()으로 대체
	//FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	FString GetHpStatText();
	
	void UpdateHpBar(float NewCurrentHp);


	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	//14-5
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	//14-5
	UPROPERTY()
	float CurrentHp;
	
	UPROPERTY()
	float MaxHp;
};





























