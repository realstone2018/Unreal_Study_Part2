// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

//싱글톤 Get함수가 실패한 경우
DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UABGameSingleton();
	static UABGameSingleton& Get();

//Character Stat Data Section
public:
	//레벨에 해당하는 구조체 반환, 유효한 데이터가 없다면 기본 구조체 생성해서 반환
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat();}

	UPROPERTY()
	int32 CharacterMaxLevel;
	
private:
	//테이블의 데이터들 저장
	TArray<FABCharacterStat> CharacterStatTable;









};
