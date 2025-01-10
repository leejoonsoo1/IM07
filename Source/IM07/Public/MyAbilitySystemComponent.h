#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

UCLASS()
class IM07_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// 초기 이펙트 적용 한번만 했는지 체크하려고 만든 변수
	bool StartUpEffectSpplied = false;
};
