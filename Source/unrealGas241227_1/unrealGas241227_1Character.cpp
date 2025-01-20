// Copyright Epic Games, Inc. All Rights Reserved.

#include "unrealGas241227_1Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AunrealGas241227_1Character

AunrealGas241227_1Character::AunrealGas241227_1Character()
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
		CreateDefaultSubobject
		<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	//온라인 사용 여부 true가 온라인
	AbilitySystemComponent->SetIsReplicated(true);

	//능력치 변경시 이벤트 호출 여부
	AbilitySystemComponent->SetReplicationMode(
		EGameplayEffectReplicationMode::Mixed);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AunrealGas241227_1Character::BeginPlay()
{
	Super::BeginPlay(); //부모한번

	//생성자에서 생성 했음.
	if (IsValid(AbilitySystemComponent))
	{
		//데이터 에셋을 에디터에서 넣은걸 여기서 UMyAttributeSet타입으로
		AttributeSetVar = AbilitySystemComponent->GetSet<UMyAttributeSet>();
		if (AttributeSetVar != nullptr)
		{
			//델리게이트로 HP 변경시 원하는 함수 호출 가능하도록
			const_cast<UMyAttributeSet*>(AttributeSetVar)->HealthChaneDelegate.AddDynamic(this,&AunrealGas241227_1Character::OnHealthChangeNative);

			InitializeAttribute();
			AddStartupEffects();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing AbilitySystemComponent."),
			*FString(__FUNCTION__));//호출한 함수 이름으로에러 메시지 출력
	}


}

void AunrealGas241227_1Character::NotifyControllerChanged()
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

void AunrealGas241227_1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AunrealGas241227_1Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AunrealGas241227_1Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

UMyAbilitySystemComponent* AunrealGas241227_1Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AunrealGas241227_1Character::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 Level)
{
	//여러개 삭제
	TArray<struct FGameplayAbilitySpec*>  MatchingAbilities;

	//현재 가지고있는 태그를 비교해서 매개변수로 넣어준 컨테이너와 일치하는게 있으면 가져옴
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		TagContainer, MatchingAbilities, true);

	//돌아가면서 안에있는 레벨 변경
	for (FGameplayAbilitySpec* spec : MatchingAbilities)
	{
		spec->Level = Level;
	}
}

void AunrealGas241227_1Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (IsValid(AbilitySystemComponent))
	{
		//어빌리티 시스템에서 해당 시스템을 사용하는 액터를 불러올수 있도록 전달.
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitalizeAbilityMulti(InitialAbilities,1);//에디터에서 설정한 스킬, 전부 레벨 1로
	}
}
void AunrealGas241227_1Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//서버에서도 알맞은 엑터를 쓸려면 여기서도 해야됨
	if (IsValid(AbilitySystemComponent))
	{
		//어빌리티 시스템에서 해당 시스템을 사용하는 액터를 불러올수 있도록 전달.
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AunrealGas241227_1Character::InitializeAttribute()
{
	if (!IsValid(AbilitySystemComponent))
		return;
	if (!IsValid(DefaultAttributes))
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing DefaultAttributes."),
			*FString(__FUNCTION__));//호출한 함수 이름으로에러 메시지 출력

		return;
	}

	//이펙트 핸들 생성
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);//어떤 놈에게 적용할지

	FGameplayEffectSpecHandle NewHandle = 
		AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0, EffectContext);

	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
			*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void AunrealGas241227_1Character::AddStartupEffects()
{
	if (!IsValid(AbilitySystemComponent) ||
		GetLocalRole() != ROLE_Authority ||
		AbilitySystemComponent->StartUpEffectApplied)
		return;

	//이펙트 핸들 생성
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);//어떤 놈에게 적용할지
	
		for (TSubclassOf<class UGameplayEffect> GameplayEffect : StartUpEffects)
		{
			FGameplayEffectSpecHandle NewHandle =
				AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContext);
			if (NewHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get(), AbilitySystemComponent);
			}
				
		}
	AbilitySystemComponent->StartUpEffectApplied = true;
}


void AunrealGas241227_1Character::InitalizeAbility(
	TSubclassOf<class UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	//온라인 상태에서 서버일때만 어빌리티 추가
	//서버 아니면 하나마나 의미없음.
	if (HasAuthority()) 
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(AbilityToGet, AbilityLevel));
	}
}

void AunrealGas241227_1Character::InitalizeAbilityMulti(
	TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel)
{
	if (HasAuthority())
	{	//돌면서 다추가
		for (TSubclassOf<class UGameplayAbility> AbilityItem : AbilityToAcquire)
		{
			InitalizeAbility(AbilityItem, AbilityLevel);
		}
	}
}

void AunrealGas241227_1Character::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	//여러개 삭제
	TArray<struct FGameplayAbilitySpec*>  MatchingAbilities;

	//현재 가지고있는 태그를 비교해서 매개변수로 넣어준 컨테이너와 일치하는게 있으면 가져옴
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		TagContainer, MatchingAbilities,true);

	//돌아가면서 삭제
	for (FGameplayAbilitySpec* spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(spec->Handle);
	}
}

void AunrealGas241227_1Character::CancelAbilityWithTags(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag)
{
	AbilitySystemComponent->CancelAbilities(&WithTag, &WithoutTag);
}

void AunrealGas241227_1Character::AddLooseGamePlayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComponent->SetTagMapCount(TagToAdd, 1);
}

void AunrealGas241227_1Character::RemoveLooseGamePlayTag(FGameplayTag TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}



void AunrealGas241227_1Character::OnHealthChangeNative(float Health, int32 StackCount)
{
	//BlueprintImplementableEvent함수라 여기서 부르면 블루프린트에서 불러짐.
	OnHealthChange(Health, StackCount);
	if (Health <= 0)
	{
		//죽음.
		Die();
	}

}
void AunrealGas241227_1Character::HealthValues(float& Health, float& MaxHealth)
{
	if (IsValid(AttributeSetVar))
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = 1000.f; //임시, 나중에 추가해야됨.
	}
}

float AunrealGas241227_1Character::GetHealth() const
{
	if (IsValid(AttributeSetVar))
		return AttributeSetVar->GetHealth();
	else
		return 0.f;
} 

float AunrealGas241227_1Character::GetMaxHealth() const
{
	return 1000.f; //임시, 나중에 추가해야됨.
}

void AunrealGas241227_1Character::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	if (IsValid(AbilitySystemComponent))
	{
		//실행중인 어빌리티 다 취소
		AbilitySystemComponent->CancelAbilities();
		
		//Die태그를 캐릭터에 붙힌다.
		FGameplayTag DieEffectTag = FGameplayTag::RequestGameplayTag(FName("Die"));

		FGameplayTagContainer gameplayTag{ DieEffectTag };
		
		//Die 태그인 어빌리티가 있으면 그거 실행
		bool IsSuccess = AbilitySystemComponent->TryActivateAbilitiesByTag(gameplayTag);
		if (IsSuccess == false) //그게 없으면 우리가 태그만 넣어줌
		{
			AbilitySystemComponent->AddLooseGameplayTag(DieEffectTag);
			FinishDying();
		}
	}

}


void AunrealGas241227_1Character::Move(const FInputActionValue& Value)
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

void AunrealGas241227_1Character::Look(const FInputActionValue& Value)
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