// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "MyAbilitySystemComponent.h"//�����Ƽ �ý���
#include "MyAttributeSet.h" //��Ʈ��Ʈ

#include "unrealGas241227_1Character.generated.h"

class UMyAbilitySystemComponent; //Ŭ���� ���漱��
class UMyAttributeSet;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AunrealGas241227_1Character : public ACharacter
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
	AunrealGas241227_1Character();
	

public: //���� �߰�
	//�����Ƽ �ý��� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	class UMyAbilitySystemComponent* AbilitySystemComponent;
	//Get�Լ�
	virtual class UMyAbilitySystemComponent* GetAbilitySystemComponent() const;

	//ĳ���� ���� ����(HP,MP,Damage�� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const class UMyAttributeSet* AttributeSetVar;

	//EditDefaultsOnly �����Ϳ��� �ʱ⿡ ĳ������ ��ų�� �߰�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//��ų ���� ���� �����Ƽ, �ʱ� �ɷ�ġ ����
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	//�⺻ ü��ȸ��, ����ȸ�� ������.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;

	//�÷��̾ �ش� ĳ���� �������� ������� ��Ʈ�ѷ�
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override; //ĳ���� ���°� ��������� ȣ���
public: //��ų ���� �Լ�

	void InitializeAttribute();
	void AddStartupEffects();


	//��ų �����Ƽ �ϳ� �ʱ�ȭ
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbility(
		TSubclassOf<class UGameplayAbility> AbilityToGet,
		int32 AbilityLevel);

	//��ų �����Ƽ ������ �ѹ��� �ʱ�ȭ
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbilityMulti(
		TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire,
		int32 AbilityLevel);



public: //�����Ƽ �±� �ý��� ����,

	//FGameplayTagContainer�� �±� ������ ����������.
	//�±� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//�±� ���
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void CancelAbilityWithTags(FGameplayTagContainer WithTag,
							   FGameplayTagContainer WithoutTag);

	//�±� �ϳ� �߰�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void AddLooseGamePlayTag(FGameplayTag TagToAdd);

	//�±� �ϳ��� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveLooseGamePlayTag(FGameplayTag TagToRemove);


	//�±� ���� ���� ������ LOL�� ġ�� ��ų����,QWER
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer,int32 Level);


public: //ĳ���� �Ӽ� ���� ��� �߰�

		UFUNCTION() //ü�� ����ɶ� �ҷ����� �Լ�
		void OnHealthChangeNative(float Health, int32 StackCount);

		//BlueprintImplementableEvent �������Ʈ���� �̺�Ʈ �߻�
		// ��� c++���� �ȸ���� ���� Native�� c++ ������
		UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
		void OnHealthChange(float Health, int32 StackCount);

		//���� ü���� �ٷ� �������°�, pure�� ���� ����
		UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
		void HealthValues(float& Health, float& MaxHealth);

		//�׳� �θ�
		UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
		float GetHealth() const;

		UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
		float GetMaxHealth() const;


public:
	void Die();

	//���� �����Ƽ
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
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

