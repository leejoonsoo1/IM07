#include "MyAttributeSet.h"
#include "Net/UnrealNetwork.h"			// 언리얼에 네트워크 통신
#include "GameplayEffect.h"				// 능력치 전달용 이펙트, 태그, 버프, 디버프
#include "GameplayEffectExtension.h"	// 확장용

UMyAttributeSet::UMyAttributeSet()
{
}

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

// 이 함수가 있어야 Multicast 수신 가능.
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

// 실질적으로 호출되는 이벤트 확인하는 부분.
void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 체력이 변경된거면
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		// GetHealth() 여기 변경된 값 들어감.
		// 0 ~ 1000 사이로 체력 설정
		SetHealth(FMath::Clamp(GetHealth(), 0.f, 1000.f));
		HealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
		// 체력 변경시 델리게이트에 있는 모든 함수에 체력 전송 멀티캐스트
	}
}