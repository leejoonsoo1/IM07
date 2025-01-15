#include "MyPlayerController.h"
#include "MyHUD.h"
#include "unrealGas241227_1/unrealGas241227_1Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// ��Ʈ�ѷ� ������ ������ �ƴҶ��� �� �����.
	if (!IsLocalController())
		return;

	AHUD* hud = GetHUD();

	if (IsValid(hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(aPawn));
		}
	}
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// ��Ʈ�ѷ� ������ ������ �ƴҶ��� �� �����.
	if (!IsLocalController())
		return;


	AHUD* hud = GetHUD();

	if (IsValid(hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AunrealGas241227_1Character>(GetCharacter()));
		} 
	}
}
