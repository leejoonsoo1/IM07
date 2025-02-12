// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "Net/UnrealNetwork.h" //�𸮾� ��Ű���, ��Ƽĳ��Ʈ ������
#include "GameplayEffect.h" //�ɷ�ġ ���޿� ����Ʈ,����,�����,ü�º��� ������ 
#include "GameplayEffectExtension.h" //Ȯ��
void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	//�̺�Ʈ �������� �̰Ŷ� �ؿ�  GetLifetimeReplicatedProps�Լ� �־�� ��Ƽĳ��Ʈ ����
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,Health,OldHealth);
}
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health,COND_None,REPNOTIFY_Always);
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//����Ʈ ����� �ش� �ϴ� ����Ʈ�� ���� Ȯ���� ó��
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) //����� ������ Ÿ�� Ȯ��
	{
		//FMath::Clamp -> 0~1000����, ���� 1000.f�� MaxHealth ��� �׳�
		SetHealth(FMath::Clamp(GetHealth(), 0.f, 1000.f));

		//HealthChangeDelegate�� ���� �Լ��� ���� ����
		HealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
	}
}
