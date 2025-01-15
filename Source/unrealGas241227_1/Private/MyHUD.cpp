#include "MyHUD.h"
#include "HUDWidget.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"
#include "Blueprint\UserWidget.h"
#include "MyPlayerController.h"

void AMyHUD::CreateHUD()
{
	// �̹� ������ ����.
	if (UIHUDWidget != nullptr)
		return;
	
	// �����Ϳ��� ���� ���� Ŭ���� ������ �־�ߵ�.
	if (UIHudWidgetClass == nullptr)
	{
		// ȣ���� �Լ� �̸����� �����޼��� ���.
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass."), *FString(__FUNCTION__));
		return;
	}

	// ��������
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
	UIHUDWidget->AddToViewport(); // ����Ʈ�� �־ ȭ�� ���

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
