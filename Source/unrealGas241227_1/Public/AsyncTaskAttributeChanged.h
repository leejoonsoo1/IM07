// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"

//��������Ʈ �Ķ���� 3��¥��, �Ӽ��� ����� �ش� �����÷��� ��Ʈ����Ʈ��
//�ٲﰪ�̶� �ٲ�� ������ ������ �ش�.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged,FGameplayAttribute,attribute,float,NewValue,float,OldValue);

//�������Ʈ ��忡�� �ڵ����� ������ ����ؼ� �Ӽ�������� ȣ���ϰ�
//UBlueprintAsyncActionBase�� �������Ʈ���� �ƹ����� ȣ�� ������ Ÿ������.
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UNREALGAS241227_1_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	//�������Ʈ���� �Լ� �������ټ� ����.
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;//�Լ� ������

	//static�� �ƹ����� ȣ�� ����, ���Լ����� UAsyncTaskAttributeChanged�� �����ؼ� ��ȯ��
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributeChange(
		UAbilitySystemComponent* AbilitySystemComponent,
		FGameplayAttribute Attribute);

	//�Ӽ��� ������ ����
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributesChange(
		UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

	//�񵿱�� ���������� ����
	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor; //�� ����ִ¾� �Ѱ� 

	TArray<FGameplayAttribute> AttributesToListenFor; //������ �Ȱ�����

	//�� ����� ����Ⱦ� ���� �ش�.
	void AttributeChanged(const FOnAttributeChangeData& Data);

};
