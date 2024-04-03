// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =  Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	//콜백으로 넘길 람다 생성
	FAICharacterAttackFinished OnAttackFinished ;
	OnAttackFinished.BindLambda(
		[&]()
		{
			//테스크가 종료되었음을 알림
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);		
		}
	);
	//델리게이트 Set함수 호출
	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	
	//12-6: 공격 애니메이션 진행, 타격 판정까지 끝나야 Attack이 끝났다가 할수 있다.
	//		 공격의 종료 판정을 틱함수가 아닌, 델리게이트로
	//공격 진행, 공격이 진행중이니 InProgress로 설정
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}
























