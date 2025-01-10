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
	
	// Get 함수
	virtual UMyAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	// 캐릭터 관련 정보(HP, MP, Damage 등 보관)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const UMyAttributeSet* AttributeSetVar;

	// EditDefaultsOnly 에디터에서 초기에 캐릭터의 스킬을 추가
	UPROPERTY(EditDefaultsOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	// 스킬 관련 게임 어빌리티, 초기 능력치 세팅
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// 기본 체력회복, 마력회복 같은거.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<UGameplayEffect>> StartUpEffects;

	// 플레이어가 해당 캐릭터 조종권한 얻었을때.
	virtual void PossessedBy(AController* NewController) override;
	// 캐릭터 상태가 변경됐을때 호출됨.
	virtual void OnRep_PlayerState() override; 

public: // 스킬 관련 함수

	void InitializeAttribute();
	void AddStartupEffects();

	// 스킬 어빌리티 하나 초기화
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	// 스킬 어빌리티 여러개 한 번에 초기화
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void InitializeAbilityMuti(TArray<TSubclassOf<UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel);
	
public: // 어빌리티 태그 시스템 관련.
	// 태그 삭제
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void RemoveAbilityWidthTags(FGameplayTagContainer TagContainer);

	// 태그 취소
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void CancelAbilityWithTag(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag);

	// 태그 하나 추가
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void AddLooseGamePlayTag(FGameplayTag TagToAdd);

	// 태그 하나만 삭제
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void RemoveLooseGamePlayTag(FGameplayTag TagToRemove);

	// 태그 레벨 변경 레벨은 LOL로 치면 스킬레벨, QWER
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 Level);

public: // 캐릭터 속성 관련 기능 추가
	UFUNCTION() // 체력 변경될때 불러지는 함수.
	void OnHealthChangeNative(float Health, int32 StackCount);

	// BlueprintImplementableEvent 블루프린트에서 이벤트 발생 
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
	void OnHealthChange(float Health, int32 StackCount);

	// 현재 체력을 바로 가져오는거, pure는 구현 없음.
	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
	void HealthValues(float& Health, float& MaxHealth);

	// 그냥 부름
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

