#include "MyPlayerController.h"
#include "MyHUD.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AHUD* hud = GetHUD();

	if (IsValid(hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(hud);
		if (IsValid(MyHud))
		{
			// 수정 해야함.
			if (IsLocalController())
			{
				MyHud->CreateHUD();
				MyHud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(aPawn));
			}
		}
	}
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AHUD* hud = GetHUD();

	if (IsValid(hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(hud);
		if (IsValid(MyHud))
		{
			// 수정 해야함.
			if (IsLocalController()) 
			{
				MyHud->CreateHUD();
				MyHud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(GetCharacter()));

			}

		} 
	}
}
