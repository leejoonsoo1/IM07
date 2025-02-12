// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "MyAbilitySystemComponent.h" //�����Ƽ �ý��� ������Ʈ
#include "MyAttributeSet.h" //�Ӽ� ������ ���̺�

#include "UnrealGas241226Character.generated.h"

class UMyAbilitySystemComponent; //Ŭ���� ���漱��
class UMyAttributeSet;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUnrealGas241226Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AUnrealGas241226Character();
public:

	//�����Ƽ �ý��� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	class UMyAbilitySystemComponent* AbilitySystemComponent;
	//Get�Լ�
	virtual class UMyAbilitySystemComponent* GetAbilitySystemComponent() const;

	//ĳ���� ���� ���� ����(HP,MP,Damage �׷���) ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const class UMyAttributeSet* AttributeSetVar;

	//EditDefaultsOnly �����Ϳ��� �ʱ⿡ ĳ���� ��ų ���� �����ϵ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//�ʱ� �ɷ�ġ ����, Maxhealth, maxMana �̷��� ó���� �����Ҷ� ����
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "GASGamePlayAbility")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	//�⺻ ü�� ȸ���̳� ���� ȸ��������,
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;

public: //��ų ���� �Լ�

	//�⺻ �ɷ�ġ �ʱ�ȭ
	void InitializeAttribute();
	void AddStartUpEffects();



	//��ų �����Ƽ �ϳ� �߰�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbility(
		TSubclassOf<class UGameplayAbility> AbilityToGet,
		int32 AbilityLevel);

	//��ų �����Ƽ ������ �߰�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbilityMulti(
		TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire,
		int32 AbilityLevel);

	//�÷��̾ �ش� ĳ���� ���� �������
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;//�¶��ο��� ĳ���� ���� �����


public: //�����Ƽ �±� �ý���

	//�±� ������ ���� FGameplayTagContainer�� �±� ������ �������ִ�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);
	
	//�±� ���� ���� LOL ��ų����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 Level);

	//�±� �߰����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void CancelAbilityWithTags(FGameplayTagContainer WithTag,
								FGameplayTagContainer WithOutTags);

	//�±� �߰�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void AddLooseGameplayTag(FGameplayTag TagToAdd);

	//�±� �ϳ� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveLooseGameplayTag(FGameplayTag TagToRemove);


public: //ĳ���� �Ӽ� ����

	UFUNCTION() //ü�º���� ȣ��, c++�� �����ؼ� OnHealthChange �������Ʈ�� �θ�.
	virtual void OnHealthChangeNative(float Health, int32 StackCount);

	//BlueprintImplementableEvent �������Ʈ���� �̺�Ʈ �߻� ���Ѽ� �������Ʈ���� ��� ����
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbilityAttribute")
	void OnHealthChange(float Health, int32 StackCount);

	//BlueprintPure ���� ü���� �������Ʈ���� �ٷ� Ȯ�� 
	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbilityAttribute")
	void HealthValues(float& Health, float& MaxHealth);

	//�������Ʈ���� ȣ�� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityAttribute")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityAttribute")
	float GetMaxHealth() const;


public:
	//�ڵ忡�� �׳� ȣ���� �Լ�
	void Die();

	//�׾����� �θ� �Լ� �������Ʈ���� Ȯ�� ����
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbilityAttribute")
	void FinishDying();


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
protected:

	virtual void BeginPlay();
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

