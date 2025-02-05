// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskAttributeChanged.h"

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	//static함수 호출할때마다 내꺼랑 같은 클래스를 새로 생성
	UAsyncTaskAttributeChanged* WaitforAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();
	
	//내가 매개변수로 넣어준 애를 세팅
	WaitforAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitforAttributeChangeTask->AttributeToListenFor = Attribute;

	//파라미터 잘못넣으면 그냥 종료
	if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
	{
		//생성한 오브젝트 다시 지운다. 언리얼 꺼라 알아서 나중에 지워짐.
		WaitforAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}

	//AbilitySystemComponent에 속성 변경시 우리꺼 AttributeChanged이거 부르게 비동기로
	//AbilitySystemComponent에서 제공해주는 기본 기능
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(Attribute).AddUObject(WaitforAttributeChangeTask, &UAsyncTaskAttributeChanged::AttributeChanged);

	return WaitforAttributeChangeTask;
}

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	//static함수 호출할때마다 내꺼랑 같은 클래스를 새로 생성
	UAsyncTaskAttributeChanged* WaitforAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();

	//내가 매개변수로 넣어준 애를 세팅
	WaitforAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitforAttributeChangeTask->AttributesToListenFor = Attributes;

	//파라미터 잘못넣으면 그냥 종료
	if (!IsValid(AbilitySystemComponent) || Attributes.Num() < 1)
	{
		//생성한 오브젝트 다시 지운다. 언리얼 꺼라 알아서 나중에 지워짐.
		WaitforAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}

	for (FGameplayAttribute Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(Attribute).AddUObject(WaitforAttributeChangeTask, &UAsyncTaskAttributeChanged::AttributeChanged);
	}

	return WaitforAttributeChangeTask;
}

void UAsyncTaskAttributeChanged::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);

	}

	//생성된 테스트 삭제
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
