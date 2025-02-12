// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS241226_API AMyHUD : public AHUD
{
	GENERATED_BODY()
protected:


	//�����Ϳ��� TSubclassOf �ش� �ڷ��� Ŭ������ �巡�� �����ϰ�
	//�츮�� �����Ϳ� �־��� UIŬ���� ����.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|UI")
	TSubclassOf<class UHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "RPG|UI")
	class UHUDWidget* UIHUDWidget;//���� �������� ������ UI
public:
	class UHUDWidget* GetHUD();
	void CreateHUD();

	void SpawnPlayerStateSetting(class AUnrealGas241226Character* Player);

};
