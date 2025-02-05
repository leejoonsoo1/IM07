// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"
#include "MyHUD.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AHUD* hud = GetHUD();
	if (IsValid(hud))
	{
		AMyHUD* myhud = Cast<AMyHUD>(hud);
		if (IsValid(myhud))
		{
			if (IsLocalController())
				myhud->CreateHUD();

			myhud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(aPawn));
		}
	}

}
void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//��Ʈ�ѷ� ������ ������ �ƴҶ��� �ȸ����

	AHUD* hud = GetHUD();
	if (IsValid(hud))
	{
		AMyHUD* myhud = Cast<AMyHUD>(hud);
		if (IsValid(myhud))
		{
			if (IsLocalController())
				myhud->CreateHUD();

			myhud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(GetCharacter()));
		}
	}
}
