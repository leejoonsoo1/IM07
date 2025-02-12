// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "HUDWidget.h"
#include "UnrealGas241226/UnrealGas241226Character.h"

UHUDWidget* AMyHUD::GetHUD()
{
	return UIHUDWidget;
}

void AMyHUD::CreateHUD()
{
	if (UIHUDWidget != nullptr) //�̹� �������� ������
		return;

	//�����Ϳ��� �巡���ؼ� �־�� �Ѵ�. ������ �־�� 
	if (UIHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing UIHUDWidgetClass."),
			*FString(__FUNCTION__));
		return;
	}
	//UIHUDWidgetClass ���� Ŭ���� �������� ���� ������ 
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();//����Ʈ�� �߰� ���ָ� ȭ�鿡 ���´�.
}

void AMyHUD::SpawnPlayerStateSetting(AUnrealGas241226Character* Player)
{
	//�Ѵ� ���� �� ������
	if (Player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(Player);
		UIHUDWidget->SetCurrentHealth(Player->GetHealth());
		UIHUDWidget->SetMaxHealth(Player->GetMaxHealth());
	}

}
