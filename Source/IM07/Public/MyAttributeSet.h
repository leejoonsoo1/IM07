#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayEffectModCallbackData;

// 델리게이트 파라미터 두 개짜리 속성 변경될때 속성 값(float), StackCount (뭔지모름) 이거 두 개 준다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangeDelegate, float, Attr, int32, StackCount);

UCLASS()
class IM07_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UMyAttributeSet();

	// Health 값이 변경될때 ReplicatedUsing 속성에 넣어준 함수 실행.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health); // 체력관리 함수 다만들어줌.

	// 현재 체력 변경시 호출할 함수 델리게이트
	FAttributeChangeDelegate HealthChangeDelegate;

	// 델리게이트랑 세트, 이거 있어야 호출 가능.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
};
