#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class AunrealGas241227_1Character;

UCLASS()
class UNREALGAS241227_1_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// BlueprintImplementable Event : 블루프린트에서 코드 구현.
	// BlueprintCallable 블루프린트에서 c++ 함수 호출 가능.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPlayer(AunrealGas241227_1Character* Player);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
};
