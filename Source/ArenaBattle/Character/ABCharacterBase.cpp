// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"	//2-4
#include "GameFramework/CharacterMovementComponent.h"	//2-4
#include "ABCharacterControlData.h" //3-4
#include "Animation/AnimMontage.h" //5-3
#include "ABComboActionData.h"	//5-6
#include "Physics/ABCollision.h"	//6-3
#include "Engine/DamageEvents.h"
#include "CharacterStat/ABCharacterStatComponent.h" //7-4
#include "UI/ABWidgetComponent.h"	//7-5
#include "UI/ABHpBarWidget.h"	//7-6
//#include "Item/ABWeaponItemData.h"	//8-6
#include "Item/ABItems.h"	//14-4

//8-6
DEFINE_LOG_CATEGORY(LogABCharacter)	;

// Sets default values
AABCharacterBase::AABCharacterBase()
{
	//2-4: 폰 기본 설정, 회전 관련 세팅
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//2-4: 폰 기본 설정, Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//2-4 -> 6-5: 캡슐 콜리전으로 변경
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	//2-4: 폰 기본 설정, Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//2-4: 폰 기본 설정, Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//2-4 -> 6-5: 캡슐 콜리전으로 변경
	//GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	////2-4: 폰 기본 설정, 스켈레탈 메쉬 컴포넌트에 에셋 세팅  -> 4-2
	//메쉬 지정
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	//if (CharacterMeshRef.Object) {
	//	GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	//}
	//애니메이션 클래스 지정
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn"));
	//if (AnimInstanceClassRef.Class) {
	//	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	//}

	//4-2: 다운 받은 스켈레탈 메쉬, 생성한 AnimInstance 적용
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (CharacterMeshRef.Object) {
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class) {
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	
	//3-4: enum과 에셋 TMap에 추가
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shouler.ABC_Shouler"));
	if (ShulderDataRef.Object) {
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShulderDataRef.Object);
	}	
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));
	if (QuaterDataRef.Object) {
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	//6-5
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object) {
		ComboActionMontage = ComboActionMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack'"));
	if (ComboActionDataRef.Object) {
		ComboActionData = ComboActionDataRef.Object;
	}

	//6-7
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object) {
		DeadMontage = DeadMontageRef.Object;
	}

	//7-4
	// Stat Component
	Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));
	
	// Widget Component
	//HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	
	//7-5
	HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());	// 기능이 아닌 컴포넌트라 transform을 지정해줘야한다.
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));	// 위젯 컴포넌트 위치 조정
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class) {
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);	//위젯 Parent Class 지정
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 15.f));	//위젯이 담길 사이즈 (패널의 크기?)
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//불필요한 충돌처리 비활성화
	}

	//8-6 Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeITemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeITemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeITemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));

	//8-7
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AABCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AABCharacterBase::ApplyStat);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation= CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ProcessComboCommand()
{
	////5-4: Combo액션시 몽타주 재생 -> 5:5: 아래 ComboActionBegin으로 대체됨
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->Montage_Play(ComboActionMontage);

	//5-7
	if (CurrentCombo == 0) {
		ComboActionBegin();
		return;	//콤보시작시에는 추가 입력 플래그 동작 불필요 바로 리턴
	}

	//5-7: 타이가 유효하지 않을때(타이머가 종료되었거나 동작하지 않았을 때) 추가입력 플래그 비활성화
	if (!ComboTimerHandle.IsValid()) {
		HasNextComboCommand = false;
	}
	else {
		HasNextComboCommand = true;
	}
}

//5-7
void AABCharacterBase::ComboActionBegin()
{
	CurrentCombo = 1;

	//콤보 애니메이션 진행을 위해 이동 중지 (Run애니메이션 동작 중지)
	//중력(높이값)도 고정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//몽타주 재생
	//10-6
	//const float AttackSpeedRate = 1.0f;
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	//몽타주 종료 델리게이트 등록
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	//5-7: 콤보 시작시 타이머 초기화 후 가동
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

//5-7
void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);

	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//12-7
	NotifyComboActionEnd();
}

void AABCharacterBase::NotifyComboActionEnd()
{
}

//5-7
void AABCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	//10-6
	//const float AttackSpeedRate = 1.0f;
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	
	//몽타주 재생 소요시간 계산
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f) {
		//ComboEffectiveTime후 this, &AABCharacterBase::ComboCheck  현재 클래스의 해당 함수 호출, 반복 호출여부는 false
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

//5-7
void AABCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	//추가 입력이 있는 경우, 다음 몽타주 재생
	if (HasNextComboCommand) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

		//재생할 몽타주 Section의 이름 파싱
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

		//다음 section의 몽타주를 지정하여 재생
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		
		SetComboCheckTimer();

		//추가입력 플래그 false
		HasNextComboCommand = false;

	}
}

//6-2, 6-3
void AABCharacterBase::AttackHitCheck()
{
	//식별용 테그, 복잡한 메쉬의 충돌체 감지여부, 무시할 엑터
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	//10-6
	// const float AttackRange = 40.0f;
	// const float AttackDamage = 100.0f;
	//12-6
	//const float AttackRadius = 50.0f;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackDamage = Stat->GetTotalStat().Attack;
	
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected) {
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	//6-4
#if ENABLE_DRAW_DEBUG
	//센터
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	//둥근부분을 뺀 길이의 절반
	float CapsuleHalfHeight = AttackRange * 0.5f;
	
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);

#endif

}

//6-7
float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//SetDead();
	//7-6
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AABCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AABCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
	UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);
	if (HpBarWidget) {
		//10-6
		//HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		//14-5
		//HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
		//14-5
		Stat->OnStatChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateStat);
	}
}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
	if (InItemData) {
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AABCharacterBase::DrinkPotion(UABItemData* InItemData)
{
	//14-4
	UABPotionItemData* PotionItemData = Cast<UABPotionItemData>(InItemData);
	if (PotionItemData)
	{
		Stat->HealHp(PotionItemData->HealAmount);
	}
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	//아이템데이터 -> 웨폰 데이터로 형변환
	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
	//if (WeaponItemData) {
	//	Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh);
	//}
	//8-8 소프르레퍼런싱으로 로딩 여부를 확인후 안되었을 경우 로드
	if (WeaponItemData) {
		if (WeaponItemData->WeaponMesh.IsPending()) {
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}

		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());

		//10-9
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
	UABScrollItemData* ScrollItemData = Cast<UABScrollItemData>(InItemData);
	if (ScrollItemData)
	{
		Stat->AddBaseStat(ScrollItemData->BaseStat);
	}
}

int32 AABCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AABCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AABCharacterBase::ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed; 
}

























