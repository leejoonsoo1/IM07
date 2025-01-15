#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UHUDWidget;
class AunrealGas241227_1Character;

UCLASS()
class UNREALGAS241227_1_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:

	// TSubclassOf �ش� Ŭ������ �巡�� �����ϰ� UHUDWidget �츮�� ����� ��� ���� �ָ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|UI")
	TSubclassOf<UHUDWidget> UIHudWidgetClass; // ����.

	UPROPERTY(BlueprintReadWrite, Category = "RPG|UI")
	UHUDWidget* UIHUDWidget; // ���� ���� �������� ������ UI

public:
	void CreateHUD();
	UHUDWidget* GetHUD();
	void SpawnPlayerStateSetting(AunrealGas241227_1Character* Player);
};
