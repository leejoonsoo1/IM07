// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "HUDWidget.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"

void AMyHUD::CreateHUD()
{
	if (UIHUDWidget != nullptr) //이미 있으면 안함
		return;

	//에디터에서 넣은 원본 클래스 무조건 있어야됨
	if (UIHudWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing UIHudWidgetClass."),
			*FString(__FUNCTION__));//호출한 함수 이름으로에러 메시지 출력
		return;
	}

	//위젯 만들어서
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
	UIHUDWidget->AddToViewport(); //뷰포트에 넣어서 화면 출력
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
