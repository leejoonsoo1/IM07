// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGas241226Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPlayerController.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUnrealGas241226Character

AUnrealGas241226Character::AUnrealGas241226Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//어빌리티 시스템 컴포넌트 생성해서 추가
	AbilitySystemComponent =
		CreateDefaultSubobject<UMyAbilitySystemComponent>
		(TEXT("AbilitySystemComponent"));

	//온라인 연동 사용 여부 true 온라인 하겠다.
	AbilitySystemComponent->SetIsReplicated(true);

	//능력치 변동시 이벤트를 호출할지 여부
	AbilitySystemComponent->SetReplicationMode(
		EGameplayEffectReplicationMode::Mixed);

}

UMyAbilitySystemComponent* AUnrealGas241226Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AUnrealGas241226Character::InitializeAttribute()
{
	if (!IsValid(AbilitySystemComponent))
		return;

	if (!IsValid(DefaultAttributes))
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing DefaultAttributes."),
			*FString(__FUNCTION__));
		return;
	}
	FGameplayEffectContextHandle EffectContext = 
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = 
		AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributes,0, EffectContext);

	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
			*NewHandle.Data.Get(), AbilitySystemComponent);
	}

}

void AUnrealGas241226Character::AddStartUpEffects()
{
	if (!IsValid(AbilitySystemComponent))
		return;

	FGameplayEffectContextHandle EffectContext =
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartUpEffects)
	{
		FGameplayEffectSpecHandle NewHandle =
			AbilitySystemComponent->MakeOutgoingSpec(
				GameplayEffect, 0, EffectContext);

		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void AUnrealGas241226Character::InitalizeAbility(
	TSubclassOf<class UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	//온라인일때 서버만 능력치 관리하므로 서버일때만 추가
	if (HasAuthority())
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(AbilityToGet, AbilityLevel));
	}

}

void AUnrealGas241226Character::InitalizeAbilityMulti(
	TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel)
{
	if (HasAuthority())
	{
		//배열로 초기화
		for (TSubclassOf<class UGameplayAbility> AbilityItem : AbilityToAcquire)
		{
			InitalizeAbility(AbilityItem, AbilityLevel);
		}
	}
	
}

void AUnrealGas241226Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (IsValid(AbilitySystemComponent))
	{
		//어빌리티 시스템에 시스템을 사용하는 액터를 전달해서
		//어빌리티 시스템 컴포넌트에서 내 액터를 찾아올 수 있도록 전달
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		//에디터에서 전달해준 스킬 어빌리티 레벨은 전부 1
		InitalizeAbilityMulti(InitialAbilities,1);
	}

	//class AMyPlayerController* myPlayercontoller = Cast<AMyPlayerController>(NewController);
	//if (IsValid(myPlayercontoller))
	//{
	//	myPlayercontoller->SpawnPlayerStateSetting(this);
	//}
}
void AUnrealGas241226Character::OnRep_PlayerState()
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AUnrealGas241226Character::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	//가져올 태그 담을 임시 배열
	TArray<struct FGameplayAbilitySpec*> MathchingAbilities;
	//AbilitySystemComponent안에 있는 활성화 된 TagContainer이랑 동일한 애들 가져온다.
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags
	(TagContainer, MathchingAbilities, true);
	
	for (FGameplayAbilitySpec* spec : MathchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(spec->Handle);
	}
}

void AUnrealGas241226Character::ChangeAbilityLevelWithTags(
	FGameplayTagContainer TagContainer, int32 Level)
{
	//가져올 태그 담을 임시 배열
	TArray<struct FGameplayAbilitySpec*> MathchingAbilities;
	//AbilitySystemComponent안에 있는 활성화 된 TagContainer이랑 동일한 애들 가져온다.
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags
	(TagContainer, MathchingAbilities, true);

	for (FGameplayAbilitySpec* spec : MathchingAbilities)
	{
		//레벨만 변경
		spec->Level = Level;
	}
}

void AUnrealGas241226Character::CancelAbilityWithTags(
	FGameplayTagContainer WithTag, FGameplayTagContainer WithOutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTag, &WithOutTags);
}

void AUnrealGas241226Character::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComponent->SetTagMapCount(TagToAdd, 1);
}

void AUnrealGas241226Character::RemoveLooseGameplayTag(FGameplayTag TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void AUnrealGas241226Character::OnHealthChangeNative(float Health, int32 StackCount)
{
	//블루프린트 함수 호출, 블루프린트에서 기능구현
	OnHealthChange(Health, StackCount);
	if (Health <= 0)
	{
		//죽었을때 이벤트 아직 없음.
		Die();
	}

}

void AUnrealGas241226Character::HealthValues(float& Health, float& MaxHealth)
{
	if (IsValid(AttributeSetVar))
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = 1000.f; //아직 없어서 그냥 1000
	}
}

float AUnrealGas241226Character::GetHealth() const
{
	if (IsValid(AttributeSetVar))
		return AttributeSetVar->GetHealth();
	else
		return 0.f;

}

float AUnrealGas241226Character::GetMaxHealth() const
{
	return 1000.f; //아직 없어서 그냥 1000;
}

void AUnrealGas241226Character::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);
	if (IsValid(AbilitySystemComponent))
	{
		//현재 실행중일 어빌리티 모두 취소
		AbilitySystemComponent->CancelAbilities();
		//Die 태그를 만들어서 
		FGameplayTag DieEffectTag = FGameplayTag::RequestGameplayTag(FName("Die"));

		FGameplayTagContainer gameplayTag{ DieEffectTag };

		//내 어빌리티 중에 Die태그가 붙은 어빌리티가 있으면 그걸 실행해라.
		bool IsSucces = AbilitySystemComponent->TryActivateAbilitiesByTag(gameplayTag);
		if (IsSucces == false) //없으면?
		{
			//태그만 넣어줌.
			AbilitySystemComponent->AddLooseGameplayTag(DieEffectTag);
			FinishDying(); //블루프린트에 이벤트 전달
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealGas241226Character::BeginPlay()
{
	Super::BeginPlay(); //부모에 있는 비긴플레이 함수 호출
	//(AbilitySystemComponent != nullptr) 이거랑 똑같음
	if (IsValid(AbilitySystemComponent))
	{
		//AbilitySystemComponent안에 데이터 에셋을 UMyAttributeSet타입으로 가져온다.
		//데이터 에셋은 언리얼 에디터에서 우리가 만들어서 넣어준다.
		AttributeSetVar = AbilitySystemComponent->GetSet<UMyAttributeSet>();
		if (AttributeSetVar != nullptr)
		{
			//HealthChangeDelegate에 내가 만든OnHealthChangeNative 연결해서 변동시 이거 부름
			const_cast<UMyAttributeSet*>(AttributeSetVar)->HealthChangeDelegate.AddDynamic(
				this, &AUnrealGas241226Character::OnHealthChangeNative);

			InitializeAttribute();
			AddStartUpEffects();

		}
	}
	else //생성 실패
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing AbilitySystemComponent."),
			*FString(__FUNCTION__)); //호출한 함수 이름으로 에러메시지 출력
	}

	//class AMyPlayerController* myPlayercontoller = Cast<AMyPlayerController>(Controller);
	//if (IsValid(myPlayercontoller))
	//{
	//	myPlayercontoller->SpawnPlayerStateSetting(this);
	//}

}

void AUnrealGas241226Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUnrealGas241226Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealGas241226Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealGas241226Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUnrealGas241226Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUnrealGas241226Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
