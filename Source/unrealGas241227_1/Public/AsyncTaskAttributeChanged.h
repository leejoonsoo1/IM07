// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"

//델리게이트 파라미터 3개짜리, 속성값 변경시 해당 게임플레이 어트리뷰트랑
//바뀐값이랑 바뀌기 전값을 나에게 준다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged,FGameplayAttribute,attribute,float,NewValue,float,OldValue);

//블루프린트 노드에서 자동으로 리스너 등록해서 속성값변경시 호출하게
//UBlueprintAsyncActionBase는 블루프린트에서 아무때나 호출 가능한 타입으로.
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UNREALGAS241227_1_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	//블루프린트에서 함수 연결해줄수 있음.
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;//함수 포인터

	//static은 아무때나 호출 가능, 이함수에서 UAsyncTaskAttributeChanged를 생성해서 반환함
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributeChange(
		UAbilitySystemComponent* AbilitySystemComponent,
		FGameplayAttribute Attribute);

	//속성값 여러개 얻음
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UAsyncTaskAttributeChanged* ListenForAttributesChange(
		UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

	//비동기로 만들어놓은거 종료
	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor; //값 들어있는애 한개 

	TArray<FGameplayAttribute> AttributesToListenFor; //여러개 똑같은거

	//값 변경시 변경된애 정보 준다.
	void AttributeChanged(const FOnAttributeChangeData& Data);

};
