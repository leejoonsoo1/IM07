// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "HUDWidget.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"

void AMyHUD::CreateHUD()
{
	if (UIHUDWidget != nullptr) //�̹� ������ ����
		return;

	//�����Ϳ��� ���� ���� Ŭ���� ������ �־�ߵ�
	if (UIHudWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing UIHudWidgetClass."),
			*FString(__FUNCTION__));//ȣ���� �Լ� �̸����ο��� �޽��� ���
		return;
	}

	//���� ����
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
	UIHUDWidget->AddToViewport(); //����Ʈ�� �־ ȭ�� ���
}

UHUDWidget* AMyHUD::GetHUD()
{
	return UIHUDWidget;
}

void AMyHUD::SpawnPlayerStateSetting(AunrealGas241227_1Character* player)
{
	if (player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(player);
		UIHUDWidget->SetCurrentHealth(player->GetHealth());
		UIHUDWidget->SetMaxHealth(player->GetMaxHealth());
	}
}
