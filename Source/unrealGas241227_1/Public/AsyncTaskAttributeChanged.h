// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"


// ��������Ʈ �Ĥ����� 3�� ¥��, �Ӽ����� ����� �ش� �����÷��� ��Ʈ����Ʈ��
// �ٲﰪ�̶� �ٲ�� �� ���� ������ �ش�.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

// �������Ʈ ��忡�� �ڵ����� ������ ����ؼ� �Ӽ�������� ȣ���ϰ�.
// UBlueprintAyncActionBase�� �������Ʈ���� �ƹ����� ȣ�� ������ Ÿ������ ����.
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UNREALGAS241227_1_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// �������Ʈ���� �Լ� �������� �� ����.
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged; // �Լ� ������

	// static�� �ƹ����� ȣ�� ����. �� �Լ����� UASyncTaskAttributeChanged�� �����ؼ� ��ȯ��.
	// �Ӽ��� ������ ����.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

protected:
	// �񵿱�� ���������� ����
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	// �� ����ִ� �� �Ѱ�.
	FGameplayAttribute AttributeToListenFor;

	// ������ �Ȱ��� ��.
	TArray<FGameplayAttribute> AttributesToListenFor;

	void AttributeChanged(const FOnAttributeChangeData& Data);
};