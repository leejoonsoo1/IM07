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
	if (UIHUDWidget != nullptr) //이미 생성한적 있으면
		return;

	//에디터에서 드래그해서 넣어야 한다. 무조건 있어야 
	if (UIHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing UIHUDWidgetClass."),
			*FString(__FUNCTION__));
		return;
	}
	//UIHUDWidgetClass 원본 클래스 기준으로 위젯 생성후 
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();//뷰포트에 추가 해주면 화면에 나온다.
}

void AMyHUD::SpawnPlayerStateSetting(AUnrealGas241226Character* Player)
{
	//둘다 생성 잘 됐으면
	if (Player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(Player);
		UIHUDWidget->SetCurrentHealth(Player->GetHealth());
		UIHUDWidget->SetMaxHealth(Player->GetMaxHealth());
	}

}
