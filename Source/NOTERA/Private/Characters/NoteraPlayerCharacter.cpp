// Copyright. All Rights Reserved.

#include "Characters/NoteraPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/NoteraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/NoteraAttributeSet.h"
#include "Notera/NOTERA.h"
#include "Utils/DebugUtil.h"

ANoteraPlayerCharacter::ANoteraPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Disable controller-based rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character movement setup
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Setup camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Setup follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Add tag to player character for npc's detection algorithms
	Tags.Add(NoteraTags::Player);
}

UAbilitySystemComponent* ANoteraPlayerCharacter::GetAbilitySystemComponent() const
{
	const ANoteraPlayerState* CCPlayerState = Cast<ANoteraPlayerState>(GetPlayerState());
	if(!IsValid(CCPlayerState))
	{
		PRINT_DEBUG_WARNING("PlayerState invalid in GetAbilitySystemComponent");
		return nullptr;
	}

	return CCPlayerState->GetAbilitySystemComponent();
}

void ANoteraPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!IsValid(GetAbilitySystemComponent()) || !HasAuthority())
	{
		PRINT_DEBUG_WARNING("AbilitySystemComponent invalid or not authoritative in PossessedBy");
		return;
	}

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnAscInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	PRINT_DEBUG("ASC initialized on PossessedBy");

	GiveStartupAbilities();
	InitializeAttributes();

	const UNoteraAttributeSet* NoteraAttributeSet = Cast<UNoteraAttributeSet>(GetAttributeSet());
	if(!IsValid(NoteraAttributeSet))
	{
		PRINT_DEBUG_WARNING("AttributeSet invalid in PossessedBy");
		return;
	}

	const FGameplayAttribute HealthAttribute = NoteraAttributeSet->GetHealthAttribute();
	FOnGameplayAttributeValueChange HealthChangeDelegate = GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(HealthAttribute);
	HealthChangeDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
}

void ANoteraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(!IsValid(GetAbilitySystemComponent()))
	{
		PRINT_DEBUG_WARNING("AbilitySystemComponent invalid in OnRep_PlayerState");
		return;
	}

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnAscInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	const UNoteraAttributeSet* NoteraAttributeSet = Cast<UNoteraAttributeSet>(GetAttributeSet());
	if(!IsValid(NoteraAttributeSet))
	{
		PRINT_DEBUG_WARNING("AttributeSet invalid");
		return;
	}

	const FGameplayAttribute HealthAttribute = NoteraAttributeSet->GetHealthAttribute();
	FOnGameplayAttributeValueChange HealthChangeDelegate = GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(HealthAttribute);
	HealthChangeDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
}

UAttributeSet* ANoteraPlayerCharacter::GetAttributeSet() const
{
	const ANoteraPlayerState* NoteraPlayerState = Cast<ANoteraPlayerState>(GetPlayerState());
	if(!IsValid(NoteraPlayerState))
	{
		PRINT_DEBUG_WARNING("PlayerState invalid in GetAttributeSet");
		return nullptr;
	}

	return NoteraPlayerState->GetAttributeSet();
}
