// Fill out your copyright notice in the Description page of Project Settings.

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
//µ¨¸®°ÔÀÌÆ® ÆÄ¶ó¹ÌÅÍ µÎ°³Â¥¸®, ¼Ó¼º º¯°æµÉ¶§¸¶´Ù °ª ¸ÖÆ¼Ä³½ºÆ®·Î Àü¼Û
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangeDelegate, float, Attr, int32, StackCount);


UCLASS()
class UNREALGAS241226_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	//Health°ªÀÌ º¯°æ µÆÀ»¶§ ReplicatedUsing ¿©±â¿¡ ³Ö¾îÁØ ÇÔ¼ö¸¦ È£Ãâ
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health); //Ã¼·Â °ü·ÃÇÔ¼ö ¸ÅÅ©·Î

	//ÇöÀç Ã¼·Â º¯°æ½Ã È£­„ÇÒ µ¨¸®°ÔÀÌÆ®
	FAttributeChangeDelegate HealthChangeDelegate;

	void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;

};
