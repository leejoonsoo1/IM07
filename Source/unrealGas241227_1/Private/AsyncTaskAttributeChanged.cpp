// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskAttributeChanged.h"

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	//static�Լ� ȣ���Ҷ����� ������ ���� Ŭ������ ���� ����
	UAsyncTaskAttributeChanged* WaitforAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();
	
	//���� �Ű������� �־��� �ָ� ����
	WaitforAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitforAttributeChangeTask->AttributeToListenFor = Attribute;

	//�Ķ���� �߸������� �׳� ����
	if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
	{
		//������ ������Ʈ �ٽ� �����. �𸮾� ���� �˾Ƽ� ���߿� ������.
		WaitforAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}

	//AbilitySystemComponent�� �Ӽ� ����� �츮�� AttributeChanged�̰� �θ��� �񵿱��
	//AbilitySystemComponent���� �������ִ� �⺻ ���
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(Attribute).AddUObject(WaitforAttributeChangeTask, &UAsyncTaskAttributeChanged::AttributeChanged);

	return WaitforAttributeChangeTask;
}

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	//static�Լ� ȣ���Ҷ����� ������ ���� Ŭ������ ���� ����
	UAsyncTaskAttributeChanged* WaitforAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();

	//���� �Ű������� �־��� �ָ� ����
	WaitforAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitforAttributeChangeTask->AttributesToListenFor = Attributes;

	//�Ķ���� �߸������� �׳� ����
	if (!IsValid(AbilitySystemComponent) || Attributes.Num() < 1)
	{
		//������ ������Ʈ �ٽ� �����. �𸮾� ���� �˾Ƽ� ���߿� ������.
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

	//������ �׽�Ʈ ����
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
