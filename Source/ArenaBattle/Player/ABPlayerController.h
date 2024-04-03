// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"
#include "ABPlayerController.generated.h"

//15-7
DECLARE_LOG_CATEGORY_EXTERN(LogABPlayerController, Log, All);

UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

	//15-4
	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();

	//15-5
	//K2_ 접두사: 블루프린트의 전신인 kismet 기능을 의미 (관습적으로 언리얼 소스코드에서 사용됨)
	//           예제에선 블루프린트에서 호출되는 함수로 일반함수와 구분을 위해 사용
	//BlueprintImplementableEvent: 블루프린트에서 이벤트처럼 호출되기 위해
	//DisplayName = "OnScoreChagnedCpp": 블루프린트상에서의 이름, C++의 함수임을 나타내기위해 Cpp 접미사 사용
	//함수 본문을 구현하지 않아도 언리얼 엔진이 UFUNCTION 매크로를 보고 이벤트임을 확인, 자동으로 본문을 만들어 준다. 
	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnScoreChagnedCpp"))
	void K2_OnScoreChanged(int32 NewScore);	

	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	
	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	//15-8
	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);
	
protected:
	virtual void BeginPlay() override;

	//13-2
	//위젯을 생성할 때 상속시킬 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	//생성할 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

//Save Game Section
protected:
	//15-7
	//게임이 실행되는 동안에 객체가 항상 메로리에서 관리되도록 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};











