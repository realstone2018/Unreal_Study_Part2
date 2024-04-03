// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//10-6
	// MaxHp = 200.0f;
	// SetHp(MaxHp);
	CurrentLevel = 1;

	//12-6
	AttackRadius = 50.0f;

	//13-3
	bWantsInitializeComponent = true;
}

//13-3 BeginPlay -> InitializeComponent
// void UABCharacterStatComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	//10-6
// 	//SetHp(MaxHp);
// 	SetLevelStat(CurrentLevel);
// 	SetHp(BaseStat.MaxHp);
// }

void UABCharacterStatComponent::InitializeComponent()
{
	//13-3
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);

	//13-6
	//BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	
	check(BaseStat.MaxHp > 0.0f);
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	//10-6
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}


