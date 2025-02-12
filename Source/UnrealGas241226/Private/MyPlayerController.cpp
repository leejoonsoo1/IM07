// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "HUDWidget.h"
#include "UnrealGas241226/UnrealGas241226Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//SpawnPlayerStateSetting(Cast<AUnrealGas241226Character>(aPawn));
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
//	SpawnPlayerStateSetting(Cast<AUnrealGas241226Character>(GetCharacter()));
}


UHUDWidget* AMyPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void AMyPlayerController::CreateHUD()
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

	//if (HasAuthority() && IsLocalPlayerController())
	//{
	//	//Host�� ������ �ڵ�
	//}
	//else if (HasAuthority() && !IsLocalPlayerController())
	//{
	//	//Client�� ������ �ڵ�
	//	UIHUDWidget = CreateWidget<UHUDWidget>(this, UIHUDWidgetClass);
	//	if(UIHUDWidget != nullptr)
	//		UIHUDWidget->AddToViewport();//����Ʈ�� �߰� ���ָ� ȭ�鿡 ���´�.
	//}

	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHUDWidgetClass);
	if (UIHUDWidget != nullptr)
		UIHUDWidget->AddToViewport();//����Ʈ�� �߰� ���ָ� ȭ�鿡 ���´�.
	//UIHUDWidgetClass ���� Ŭ���� �������� ���� ������ 

}

void AMyPlayerController::SpawnPlayerStateSetting(AUnrealGas241226Character* _Player)
{
	CreateHUD();
	//�Ѵ� ���� �� ������
	if (_Player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(_Player);
		UIHUDWidget->SetCurrentHealth(_Player->GetHealth());
		UIHUDWidget->SetMaxHealth(_Player->GetMaxHealth());
	}

}
