// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
//#include "Player/ABPlayerController.h"  //ConstructorHelpers로 대체
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	//1-3 -> 2-4
	////생성자안에서는 ConstructorHelpers로 에셋의 정보를 가져올 수 있다.
	////에셋의 클래스 정보를 가져올거라 마지막에 _C를 추가해준다.
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class) {
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}
	//2-4: 기존 ThirdPerson을 제외 / 생성한 폰(ABCharacterBase)을 상속받는 ABCharacterPlayer로 세팅
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class) {
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	//1-3 -> 1-5
	//PlayerControllerClass = AABPlayerController::StaticClass();	//ConstructorHelpers로 대체
	//1-5
	//생성한 cpp파일도 위와같은 방법으로, include 없이 에셋으로 부터 직접 클래스 정보를 가져올 수 있다.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class; 
	}

	//15-1
	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	//15-4
	//싱글 게임임으로 첫번째 플레이어만 접근
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if(ABPlayerController)
	{
		ABPlayerController->GameScoreChanged(CurrentScore);
	}
	
	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		//15-4
		if(ABPlayerController)
		{
			ABPlayerController->GameClear();
		}
	}
}

void AABGameMode::OnPlayerDead()
{
	//15-4
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if(ABPlayerController)
	{
		ABPlayerController->GameOver();
	}
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}





























