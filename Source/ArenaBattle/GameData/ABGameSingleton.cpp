// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

//싱글톤 Get함수가 실패한 경우, 구현
DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		//인덱스를 이용한 순차적을 할거라, Key를 버리고 Value만 저장
		TArray<uint8*> ValueArray;
		//Value만 배열로 가져와, 알고리즘의 Transform라이브러리를 사용해 저장
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	//CastChecked로 형변환이 되는지 (가져온게 유요한 객체인지) 강력하게 확인
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	//싱글톤 Get함수가 실패한 경우
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	//함수 완성을 위한 return, 실제 사용되지는 않음 
	return *NewObject<UABGameSingleton>();
}
