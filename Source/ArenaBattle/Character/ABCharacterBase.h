// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"	//6-2
#include "Interface/ABCharacterWidgetInterface.h"	//7-6
#include "Interface/ABCharacterItemInterface.h"	//8-5
#include  "GameData/ABCharacterStat.h"	//14-2 
#include "ABCharacterBase.generated.h"

//8-6
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

//3-4 숄더뷰와 쿼터뷰를 관리할 열거형
UENUM()
enum class ECharacterControlType : uint8 {
	Shoulder,
	Quater
};

//8-6
DECLARE_DELEGATE_OneParam(FOnTakeITemDelegate, class UABItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper {
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeITemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeITemDelegate ItemDelegate;
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface, public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	//7-6
	virtual void PostInitializeComponents() override;

protected:
	//3-4: 인자로 받은 데이터 에셋값을 실제 캐릭터 무브먼트에 적용
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	//3-4: enum과 데이터에셋 TMap으로 관리
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

//5-3: 애니메이션 몽타주
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation) 
	TObjectPtr<class UAnimMontage> ComboActionMontage;
	
	void ProcessComboCommand();

	//5-7: 생성한 DataAsset 가져오기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	//몽타주가 시작될 때 호출
	void ComboActionBegin();

	//몽타주가 종료될 때 호출, 몽타주에 설정된 delegate로 바로 호출되도록 파라미터 세팅 (UAnimMontage.OnMontageEnd 딜리게이트의 파라미터와 동일하게)
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	//12-7 공격이 끝났을때 호출, ComboActionEnd는 몽타주가 포함되어 있어 별도로 구현
	virtual void NotifyComboActionEnd();
	
	//현재 콤보 진행 단계, 내부에서만 사용함으로 UPROPERTY 붙이지 않음
	int32 CurrentCombo = 0;

	//5-7: 타이머, 언리얼 엔진 월드의 타이머 기능 활용(FTimerHandle)
	FTimerHandle ComboTimerHandle;
	//내부에서만 사용함으로 UPROPERTY를 붙이지 않고, 그러므로 int와 비트플래그로 대체할 필요 없음
	bool HasNextComboCommand = false;
	
	void SetComboCheckTimer();
	void ComboCheck();

// Attack Hit Section
protected: 
	//6-2
	virtual void AttackHitCheck() override;
	//6-7 EventInstigator: 가해자    DamageCauser: 가해자가 빙의한 폰(ex 사용한 무기 등) 
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
// Dead Section
protected:
	//6-7
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	//6-9
	float DeadEventDelayTime = 5.0f;

// Stat Section
protected: 
	//7-4
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

// UI Widget Section
protected:
	//7-4
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UWidgetComponent> HpBar; 

	//7-5 Owner를 얻어올수있는 확장 컴포넌트로 변경 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar; 

//7-6
	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;


// Item Section
protected:
	//8-7
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	//8-5
	virtual void TakeItem(class UABItemData* InItemData) override;

	//8-6
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

	//Stat Section
public:
	//10-7 플레이어에 레벨업 기능이 없으니, ABCharacgterNonPlayer에 있는게 맞지 않나?
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

	//14-2
	void ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
};




























