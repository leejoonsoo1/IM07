// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "MyAbilitySystemComponent.h"	// Ability System
#include "MyAttributeSet.h"				// Attribute

#include "IM07Character.generated.h"

class UMyAbilitySystemComponent;
class UMyAttributeSet;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AIM07Character : public ACharacter
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
	AIM07Character();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	UMyAbilitySystemComponent* AbilitySystemComponent;
	
	// Get �Լ�
	virtual UMyAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	// ĳ���� ���� ����(HP, MP, Damage �� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const UMyAttributeSet* AttributeSetVar;

	// EditDefaultsOnly �����Ϳ��� �ʱ⿡ ĳ������ ��ų�� �߰�
	UPROPERTY(EditDefaultsOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	// ��ų ���� ���� �����Ƽ, �ʱ� �ɷ�ġ ����
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// �÷��̾ �ش� ĳ���� �������� �������.
	virtual void PossessedBy(AController* NewController) override;
	// ĳ���� ���°� ��������� ȣ���.
	virtual void OnRep_PlayerState() override; 

public: // ��ų ���� �Լ�
	// ��ų �����Ƽ �ϳ� �ʱ�ȭ
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	// ��ų �����Ƽ ������ �� ���� �ʱ�ȭ
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void InitializeAbilityMuti(TArray<TSubclassOf<UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel);
	
public: // �����Ƽ �±� �ý��� ����.
	// �±� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void RemoveAbilityWidthTags(FGameplayTagContainer TagContainer);

	// �±� ���
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void CancelAbilityWithTag(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag);

	// �±� �ϳ� �߰�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void AddLooseGamePlayTag(FGameplayTag TagToAdd);

	// �±� �ϳ��� ����
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void RemoveLooseGamePlayTag(FGameplayTag TagToRemove);

	// �±� ���� ���� ������ LOL�� ġ�� ��ų����, QWER
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 Level);

public: // ĳ���� �Ӽ� ���� ��� �߰�
	UFUNCTION() // ü�� ����ɶ� �ҷ����� �Լ�.
	void OnHealthChangeNative(float Health, int32 StackCount);

	// BlueprintImplementableEvent �������Ʈ���� �̺�Ʈ �߻� 
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
	void OnHealthChange(float Health, int32 StackCount);

	// ���� ü���� �ٷ� �������°�, pure�� ���� ����.
	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
	void HealthValues(float& Health, float& MaxHealth);

	// �׳� �θ�
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	float GetMaxHealth() const;

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

