// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();	//Super에서 UI위젯의 인스턴스가 생성됨

	//Super이후에 인스턴스에 접근 가능
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget) {
		ABUserWidget->SetOwningActor(GetOwner());	//UABUserWidget는 컴포넌트라서, GetOwner()로 소유자를 가져올 수있다.
	}
}
