// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"


//��������Ʈ �Ķ���� 3��¥��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged,FGameplayAttribute,Attribute,float,NewValue,float,OldValue);

//�������Ʈ �ε忡�� �ڵ����� �����ʸ� ����ؼ� 
//��Ʈ����Ʈ�� ��������� ȣ�����ټ� �ֵ���
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UNREALGAS241226_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged; //�Լ� ������

	//��������� �������Ʈ Ȯ�� �����ϵ��� static�̶� �ƹ����� ȣ�� ����
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributeChange(
		UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributesChange(
		UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor; //�� ����ִ¾� �ϳ� ��ٷ��� üũ
	TArray<FGameplayAttribute> AttributesToListenFor; //������

	//�� ����ɶ� �̰� ȣ��
	void AttributeChanged(const FOnAttributeChangeData& Data);

};
