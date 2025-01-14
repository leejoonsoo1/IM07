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

	//�����Ƽ �ý��� ������Ʈ �����ؼ� �߰�
	AbilitySystemComponent = 
		CreateDefaultSubobject
		<UMyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	//�¶��� ��� ���� true�� �¶���
	AbilitySystemComponent->SetIsReplicated(true);

	//�ɷ�ġ ����� �̺�Ʈ ȣ�� ����
	AbilitySystemComponent->SetReplicationMode(
		EGameplayEffectReplicationMode::Mixed);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AunrealGas241227_1Character::BeginPlay()
{
	Super::BeginPlay(); //�θ��ѹ�

	//�����ڿ��� ���� ����.
	if (IsValid(AbilitySystemComponent))
	{
		//������ ������ �����Ϳ��� ������ ���⼭ UMyAttributeSetŸ������
		AttributeSetVar = AbilitySystemComponent->GetSet<UMyAttributeSet>();
		if (AttributeSetVar != nullptr)
		{
			//��������Ʈ�� HP ����� ���ϴ� �Լ� ȣ�� �����ϵ���
			const_cast<UMyAttributeSet*>(AttributeSetVar)->HealthChaneDelegate.AddDynamic(this,&AunrealGas241227_1Character::OnHealthChangeNative);

			InitializeAttribute();
			AddStartupEffects();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s()Missing AbilitySystemComponent."),
			*FString(__FUNCTION__));//ȣ���� �Լ� �̸����ο��� �޽��� ���
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
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
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
	//������ ����
	TArray<struct FGameplayAbilitySpec*>  MatchingAbilities;

	//���� �������ִ� �±׸� ���ؼ� �Ű������� �־��� �����̳ʿ� ��ġ�ϴ°� ������ ������
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		TagContainer, MatchingAbilities, true);

	//���ư��鼭 �ȿ��ִ� ���� ����
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
		//�����Ƽ �ý��ۿ��� �ش� �ý����� ����ϴ� ���͸� �ҷ��ü� �ֵ��� ����.
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitalizeAbilityMulti(InitialAbilities,1);//�����Ϳ��� ������ ��ų, ���� ���� 1��
	}
}
void AunrealGas241227_1Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//���������� �˸��� ���͸� ������ ���⼭�� �ؾߵ�
	if (IsValid(AbilitySystemComponent))
	{
		//�����Ƽ �ý��ۿ��� �ش� �ý����� ����ϴ� ���͸� �ҷ��ü� �ֵ��� ����.
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
			*FString(__FUNCTION__));//ȣ���� �Լ� �̸����ο��� �޽��� ���

		return;
	}

	//����Ʈ �ڵ� ����
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);//� �𿡰� ��������

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

	//����Ʈ �ڵ� ����
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);//� �𿡰� ��������
	
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
	//�¶��� ���¿��� �����϶��� �����Ƽ �߰�
	//���� �ƴϸ� �ϳ����� �ǹ̾���.
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
	{	//���鼭 ���߰�
		for (TSubclassOf<class UGameplayAbility> AbilityItem : AbilityToAcquire)
		{
			InitalizeAbility(AbilityItem, AbilityLevel);
		}
	}
}

void AunrealGas241227_1Character::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	//������ ����
	TArray<struct FGameplayAbilitySpec*>  MatchingAbilities;

	//���� �������ִ� �±׸� ���ؼ� �Ű������� �־��� �����̳ʿ� ��ġ�ϴ°� ������ ������
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
		TagContainer, MatchingAbilities,true);

	//���ư��鼭 ����
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
	//BlueprintImplementableEvent�Լ��� ���⼭ �θ��� �������Ʈ���� �ҷ���.
	OnHealthChange(Health, StackCount);
	if (Health <= 0)
	{
		//����.
		Die();
	}

}
void AunrealGas241227_1Character::HealthValues(float& Health, float& MaxHealth)
{
	if (IsValid(AttributeSetVar))
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = 1000.f; //�ӽ�, ���߿� �߰��ؾߵ�.
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
	return 1000.f; //�ӽ�, ���߿� �߰��ؾߵ�.
}

void AunrealGas241227_1Character::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	if (IsValid(AbilitySystemComponent))
	{
		//�������� �����Ƽ �� ���
		AbilitySystemComponent->CancelAbilities();
		
		//Die�±׸� ĳ���Ϳ� ������.
		FGameplayTag DieEffectTag = FGameplayTag::RequestGameplayTag(FName("Die"));

		FGameplayTagContainer gameplayTag{ DieEffectTag };
		
		//Die �±��� �����Ƽ�� ������ �װ� ����
		bool IsSuccess = AbilitySystemComponent->TryActivateAbilitiesByTag(gameplayTag);
		if (IsSuccess == false) //�װ� ������ �츮�� �±׸� �־���
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