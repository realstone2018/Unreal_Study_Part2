 // Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"
 
//15-7
DEFINE_LOG_CATEGORY(LogABPlayerController);

 AABPlayerController::AABPlayerController()
 {
 	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
 	if (ABHUDWidgetRef.Class)
 	{
 		ABHUDWidgetClass = ABHUDWidgetRef.Class;
 	}
 }

 void AABPlayerController::GameScoreChanged(int32 NewScore)
 {
 	K2_OnScoreChanged(NewScore);
 }

 void AABPlayerController::GameClear()
 {
 	K2_OnGameClear();
 }

 void AABPlayerController::GameOver()
 {
 	K2_OnGameOver();

 	//15-7
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
 	{
 		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!"));
 	}

 	//15-8
 	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
 }

 void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//1-4: 게임 시작시, 마우스 커서를 뷰포트 안으로
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

 	//15-6 블루프린트로 대체
 	// //13-2
 	// ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
 	// if (ABHUDWidget)
 	// {
 	// 	ABHUDWidget->AddToViewport();	
 	// }

 	//UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0) : (저장할 파일 이름, 플레이어 번호), 싱글의 경우 항상 0
 	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
 	if (SaveGameInstance)
 	{
 		SaveGameInstance->RetryCount++;
 	}
 	else
 	{
 		SaveGameInstance = NewObject<UABSaveGame>();
 		SaveGameInstance->RetryCount = 0;
 	}

 	//15-8
 	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}























