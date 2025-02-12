// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS241226_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//BlueprintImplementableEvent �ڵ� ������ �������Ʈ���� �ϰڴ�.
	//BlueprintCallable �������Ʈ���� c++�Լ� ȣ�� �����ϰ�
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetPlayer(class AUnrealGas241226Character* player);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

};
