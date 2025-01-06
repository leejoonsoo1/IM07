#include "MyAttributeSet.h"
#include "Net/UnrealNetwork.h"			// �𸮾� ��Ʈ��ũ ���
#include "GameplayEffect.h"				// �ɷ�ġ ���޿� ����Ʈ, �±�, ����, �����
#include "GameplayEffectExtension.h"	// Ȯ���

UMyAttributeSet::UMyAttributeSet()
{
}

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

// �� �Լ��� �־�� Multicast ���� ����.
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

// ���������� ȣ��Ǵ� �̺�Ʈ Ȯ���ϴ� �κ�.
void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// ü���� ����ȰŸ�
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		// GetHealth() ���� ����� �� ��.
		// 0 ~ 1000 ���̷� ü�� ����
		SetHealth(FMath::Clamp(GetHealth(), 0.f, 1000.f));
		HealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
		// ü�� ����� ��������Ʈ�� �ִ� ��� �Լ��� ü�� ���� ��Ƽĳ��Ʈ
	}
}