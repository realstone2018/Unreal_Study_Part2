// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"	//UMG모듈에서 지원, Build.cs로 가서 UMG를 추가해줘야한다.
#include "Components/TextBlock.h"	//14-5
#include "Interface/ABCharacterWidgetInterface.h"	//7-8

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	//14-5
	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);
	
	//7-8
	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget) {
		CharacterWidget->SetupCharacterWidget(this);
	}
}

//<~> UpdateStat보다는 UpdateHp로 하고 , Hp값만 인자로 받았으면 좋았을거 같은데
void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UABHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;
	
	ensure(MaxHp > 0.0f);
	if (HpProgressBar) {
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
	
	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}





















