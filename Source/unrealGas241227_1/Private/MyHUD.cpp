#include "MyHUD.h"
#include "HUDWidget.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"
#include "Blueprint\UserWidget.h"
#include "MyPlayerController.h"

void AMyHUD::CreateHUD()
{
	// 이미 있으면 안함.
	if (UIHUDWidget != nullptr)
		return;
	
	// 에디터에서 넣은 원본 클래스 무조건 있어야됨.
	if (UIHudWidgetClass == nullptr)
	{
		// 호출한 함수 이름으로 에러메세지 출력.
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass."), *FString(__FUNCTION__));
		return;
	}

	// 위젯만들어서
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
	UIHUDWidget->AddToViewport(); // 뷰포트에 넣어서 화면 출력

}

UHUDWidget* AMyHUD::GetHUD()
{
	return UIHUDWidget;
}

void AMyHUD::SpawnPlayerStateSetting(AunrealGas241227_1Character* Player)
{
	if (Player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(Player);
		UIHUDWidget->SetCurrentHealth(Player->GetHealth());
		UIHUDWidget->SetMaxHealth(Player->GetMaxHealth());
	}
}
