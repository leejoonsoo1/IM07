// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "Net/UnrealNetwork.h" //�𸮾� ��Ʈ��ũ ���
#include "GameplayEffect.h" //�ɷ�ġ ���޿� ����Ʈ,�±�, �׷��� ���� �����
#include "GameplayEffectExtension.h" //Ȯ���
UMyAttributeSet::UMyAttributeSet()
{
}
void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,Health, OldHealth);
}
//���Լ��� �־�� multicast ���� ����
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
}
//���� ������ ȣ�� �Ǵ� �̺�Ʈ Ȯ���ϴ� �κ�
void UMyAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) //ü���� ����ȰŸ� ��
	{
		//GetHealth() ���� ����� �� ��
		SetHealth(FMath::Clamp(GetHealth(), 0.f, 1000.f));; //0~1000���̷� ü�� ����
		HealthChaneDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
		//ü�� ����� ��������Ʈ�� �ִ� ��� �Լ��� ü�� ���� ��Ƽĳ��Ʈ
	}

}
