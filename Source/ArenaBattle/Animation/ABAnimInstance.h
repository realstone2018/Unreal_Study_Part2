// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();

protected:
	// AnimInstance가 처음 생성될 때 한번 호출되는 이벤트
	virtual void NativeInitializeAnimation() override;
	
	// 프레임마다 호출되는 이벤트
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// AnimInstance를 소유한 캐릭터의 정보를 담을 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// boolean 값을 사용할때, 보통 uint8 인티저 형을 선언하되 접미사로 b를 붙인 뒤, 비트플래그를 달아준다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//움직이고 있는지를 판단할 기준 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float movingThreshould;

	//떨어지고 있는지 검색
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;
	 
	//점프하고 있는지 검색
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//점프중인지를 판단할 기준 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float jumpingThreshould;
};
