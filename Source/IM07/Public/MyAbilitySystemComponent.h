#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

UCLASS()
class IM07_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// �ʱ� ����Ʈ ���� �ѹ��� �ߴ��� üũ�Ϸ��� ���� ����
	bool StartUpEffectSpplied = false;
};
