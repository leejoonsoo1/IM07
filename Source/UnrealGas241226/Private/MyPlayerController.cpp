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
	if (UIHUDWidget != nullptr) //이미 생성한적 있으면
		return;

	//에디터에서 드래그해서 넣어야 한다. 무조건 있어야 
	if (UIHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing UIHUDWidgetClass."),
			*FString(__FUNCTION__));
		return;
	}

	//if (HasAuthority() && IsLocalPlayerController())
	//{
	//	//Host가 실행할 코드
	//}
	//else if (HasAuthority() && !IsLocalPlayerController())
	//{
	//	//Client가 실행할 코드
	//	UIHUDWidget = CreateWidget<UHUDWidget>(this, UIHUDWidgetClass);
	//	if(UIHUDWidget != nullptr)
	//		UIHUDWidget->AddToViewport();//뷰포트에 추가 해주면 화면에 나온다.
	//}

	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHUDWidgetClass);
	if (UIHUDWidget != nullptr)
		UIHUDWidget->AddToViewport();//뷰포트에 추가 해주면 화면에 나온다.
	//UIHUDWidgetClass 원본 클래스 기준으로 위젯 생성후 

}

void AMyPlayerController::SpawnPlayerStateSetting(AUnrealGas241226Character* _Player)
{
	CreateHUD();
	//둘다 생성 잘 됐으면
	if (_Player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(_Player);
		UIHUDWidget->SetCurrentHealth(_Player->GetHealth());
		UIHUDWidget->SetMaxHealth(_Player->GetMaxHealth());
	}

}
