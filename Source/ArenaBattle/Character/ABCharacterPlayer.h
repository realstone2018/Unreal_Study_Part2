// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"	//2-6: ArenaBattle.Build.cs에 EnhancedInput 모듈을 추가해줘야 헤더 Include 가능
#include "Interface/ABCharacterHUDInterface.h"	//13-5
#include "ABCharacterPlayer.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase, public IABCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	//2-6: 입력 컴포넌트 추가 및 세팅
	//입력 맵핑 컨텍스트 할당 진행
	virtual void BeginPlay() override;
	//14-1
	virtual void SetDead() override;
	
public:
	//2-6: 입력 액션과 함수를 맵핑시켜주는 역할
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//3-4: 캐릭터 컨트롤 
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;

	//3-5: V키(ChangeControl) 맵핑 함수 
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	
// Camera Section
protected:
	//2-5: 카메라 컴포넌트 추가 및 세팅
	//Meta = (AllowPrivateAccess = "true"): private로 선언된 언리얼 오브젝트를 블루프린트에서 접근할수 있게해준다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;	//카메라를 지탱해주는 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;	//실제 카메라 컴포넌트
	
// Input Section
protected:
	//2-6: 입력 컴포넌트 추가
	//다른 에셋으로 변경해주기 위해 EditAnywhere사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> JumpAction;
	
	//2-6 -> 3-5: Shoulder, Quater에 따라 변경으로 이전 디폴트값(예제 주석처리)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	//TObjectPtr<class UInputAction> MoveAction;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	//TObjectPtr<class UInputAction> LookAction;
	//void Move(const FInputActionValue& Value);
	//void Look(const FInputActionValue& Value);

	//3-5: Shoulder, Quater에 따라 변경
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ChangeControlAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> QuaterMoveAction;

	//5-2 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> AttackAction;
	void Attack();

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	// UI Section
protected:
	//13-5
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) override;
};
