// Copyright Epic Games, Inc. All Rights Reserved.

#include "IM07Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CoreMinimal.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AIM07Character

AIM07Character::AIM07Character()
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

	// 어빌리티 시스템 컴포넌트 생성해서 추가.
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// 온라인 사용 여부 true가 온라인
	AbilitySystemComponent->SetIsReplicated(true);

	// 능력치 변경시 이벤트 호출 여부
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UMyAbilitySystemComponent* AIM07Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AIM07Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		// 어빌리티 시스템에서 해당 시스템을 사용하는 액터를 불러올수 있도록 전다.
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		// 에디터에서 설정한 스킬, 전부 레벨 1로.
		InitializeAbilityMuti(InitialAbilities, 1);
	}
}

void AIM07Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 서버에서도 알맞은 액터를 쓸려면 여기서도 해야됨.
	if (IsValid(AbilitySystemComponent))
	{
		// 어빌리티 시스템에서 해당 시스템을 사용하는 액터를 불러올수 있도록 전다.
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AIM07Character::InitializeAttribute()
{
	if (IsValid(AbilitySystemComponent))
		return;

	if (IsValid(DefaultAttributes))
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Missing DefaultAttributes"), *GetNameSafe(this));

		return;
	}

	// 이펙트 핸들 생성
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // 어떤 놈에게 적용할지.

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0, EffectContext);

	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void AIM07Character::AddStartupEffects()
{
	if (IsValid(AbilitySystemComponent) || GetLocalRole() != ROLE_Authority || AbilitySystemComponent->StartUpEffectSpplied)
		return;

	// 이펙트 핸들 생성
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // 어떤 놈에게 적용할지.

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartUpEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContext);
		
		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
  
	AbilitySystemComponent->StartUpEffectSpplied = true;
}

void AIM07Character::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	// 온라인 상태에서 서버일때만 어빌리티 추가.
	// 서버 아니면 하나마나 의미없음.
	if (HasAuthority())
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel));
	}
}

void AIM07Character::InitializeAbilityMuti(TArray<TSubclassOf<UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel)
{
	if (HasAuthority())
	{
		for (TSubclassOf<UGameplayAbility> AbilityItem : AbilityToAcquire)
		{
			InitializeAbility(AbilityItem, AbilityLevel);
		}
	}
}

void AIM07Character::RemoveAbilityWidthTags(FGameplayTagContainer TagContainer)
{
	// 여러개 삭제
	TArray<FGameplayAbilitySpec*> MatchingAbilities;

	// 현재 가지고있는 태그를 비교해서 매개변수로 넣어준 컨테이너와 일치하는게 있으면 가져옴.
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);

	// 돌아가면서 삭제
	for (FGameplayAbilitySpec* spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(spec->Handle);
	}
}

void AIM07Character::CancelAbilityWithTag(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag)
{
	AbilitySystemComponent->CancelAbilities(&WithTag, &WithoutTag);
}

void AIM07Character::AddLooseGamePlayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComponent->SetTagMapCount(TagToAdd, 1);
}

void AIM07Character::RemoveLooseGamePlayTag(FGameplayTag TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void AIM07Character::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 Level)
{
	// 여러개 삭제
	TArray<FGameplayAbilitySpec*> MatchingAbilities;

	// 현재 가지고있는 태그를 비교해서 매개변수로 넣어준 컨테이너와 일치하는게 있으면 가져옴.
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);

	// 돌아가면서 레벨 변경
	for (FGameplayAbilitySpec* spec : MatchingAbilities)
	{
		spec->Level = Level;
	}
}

void AIM07Character::OnHealthChangeNative(float Health, int32 StackCount)
{
	// BlueprintImplementEvent 함수라 여기서 부르면 블루프린트에서 호출됨.
	OnHealthChange(Health, StackCount);

	if (Health <= 0)
	{
		// 죽음
	}
}

void AIM07Character::HealthValues(float& Health, float& MaxHealth)
{
	if (IsValid(AttributeSetVar))
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = 1000.f; // 임시, 나중에 추가해야됨.
	}
}

float AIM07Character::GetHealth() const
{
	return 0.0f;
}

float AIM07Character::GetMaxHealth() const
{
	return 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AIM07Character::NotifyControllerChanged()
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

void AIM07Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started,		this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed,	this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,	this, &AIM07Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,	this, &AIM07Character::Look);

		// Attacking
		//PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed,				this, &AIM07Character::Trace);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AIM07Character::BeginPlay()
{
	// 부모 호출
	Super::BeginPlay(); 

	// 생성자에서 생성이 잘 됐는지 확인.
	if (AbilitySystemComponent != nullptr)
	{
		// 데이터 에셋을 에디터에서 넣은걸 여기서 UMyAttributeSet 타입으로 캐스팅
		AttributeSetVar = AbilitySystemComponent->GetSet<UMyAttributeSet>();

		if (AttributeSetVar != nullptr)
		{
			// 델리게이트로 HP 변경시 원하는 함수 호출 가능하도록.
			const_cast<UMyAttributeSet*>(AttributeSetVar)->HealthChangeDelegate.AddDynamic(this, &AIM07Character::OnHealthChangeNative);

			InitializeAttribute();
			AddStartupEffects();
		}
	}
	else
	{
		// 호출한 함수 이름으로에러 메세지 출력.
		UE_LOG(LogTemp, Error, TEXT("%s()Missing AbilitySystemComponent."), *FString(__FUNCTION__));
	}
}

void AIM07Character::Move(const FInputActionValue& Value)
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

void AIM07Character::Look(const FInputActionValue& Value)
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
