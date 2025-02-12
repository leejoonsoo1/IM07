// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"


//델리게이트 파라미터 3개짜리
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged,FGameplayAttribute,Attribute,float,NewValue,float,OldValue);

//블루프린트 로드에서 자동으로 리스너를 등록해서 
//어트리뷰트가 변경됐을때 호출해줄수 있도록
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UNREALGAS241226_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged; //함수 포인터

	//변경됐을때 블루프린트 확인 가능하도록 static이라 아무때나 호출 가능
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

	FGameplayAttribute AttributeToListenFor; //값 들어있는애 하나 기다려서 체크
	TArray<FGameplayAttribute> AttributesToListenFor; //여러개

	//값 변경될때 이거 호출
	void AttributeChanged(const FOnAttributeChangeData& Data);

};
