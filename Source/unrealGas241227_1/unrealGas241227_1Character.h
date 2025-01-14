// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "MyAbilitySystemComponent.h"//어빌리티 시스템
#include "MyAttributeSet.h" //어트뷰트

#include "unrealGas241227_1Character.generated.h"

class UMyAbilitySystemComponent; //클래스 전방선언
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
	

public: //내꺼 추가
	//어빌리티 시스템 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	class UMyAbilitySystemComponent* AbilitySystemComponent;
	//Get함수
	virtual class UMyAbilitySystemComponent* GetAbilitySystemComponent() const;

	//캐릭터 관련 정보(HP,MP,Damage등 보관)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const class UMyAttributeSet* AttributeSetVar;

	//EditDefaultsOnly 에디터에서 초기에 캐릭터의 스킬을 추가
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//스킬 관련 게임 어빌리티, 초기 능력치 세팅
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	//기본 체력회복, 마력회복 같은거.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;

	//플레이어가 해당 캐릭터 조종권한 얻었을때 컨트롤러
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override; //캐릭터 상태가 변경됐을때 호출됨
public: //스킬 관련 함수

	void InitializeAttribute();
	void AddStartupEffects();


	//스킬 어빌리티 하나 초기화
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbility(
		TSubclassOf<class UGameplayAbility> AbilityToGet,
		int32 AbilityLevel);

	//스킬 어빌리티 여러개 한번에 초기화
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitalizeAbilityMulti(
		TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire,
		int32 AbilityLevel);



public: //어빌리티 태그 시스템 관련,

	//FGameplayTagContainer는 태그 여러개 담을수있음.
	//태그 삭제
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//태그 취소
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void CancelAbilityWithTags(FGameplayTagContainer WithTag,
							   FGameplayTagContainer WithoutTag);

	//태그 하나 추가
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void AddLooseGamePlayTag(FGameplayTag TagToAdd);

	//태그 하나만 삭제
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveLooseGamePlayTag(FGameplayTag TagToRemove);


	//태그 레벨 변경 레벨은 LOL로 치면 스킬레벨,QWER
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer,int32 Level);


public: //캐릭터 속성 관련 기능 추가

		UFUNCTION() //체력 변경될때 불러지는 함수
		void OnHealthChangeNative(float Health, int32 StackCount);

		//BlueprintImplementableEvent 블루프린트에서 이벤트 발생
		// 얘는 c++에서 안만들고 위에 Native가 c++ 구현임
		UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
		void OnHealthChange(float Health, int32 StackCount);

		//현재 체력을 바로 가져오는거, pure는 구현 없음
		UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
		void HealthValues(float& Health, float& MaxHealth);

		//그냥 부름
		UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
		float GetHealth() const;

		UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbility")
		float GetMaxHealth() const;


public:
	void Die();

	//죽음 어빌리티
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

