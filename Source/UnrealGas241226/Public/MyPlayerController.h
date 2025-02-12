// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS241226_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnRep_PlayerState() override;

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

	UFUNCTION(BlueprintCallable, Category = "RPG|UI")
	void SpawnPlayerStateSetting(class AUnrealGas241226Character* _Player);
};
