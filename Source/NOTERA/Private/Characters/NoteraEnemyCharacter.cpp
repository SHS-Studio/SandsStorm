// Copyright. All Rights Reserved.

#include "Characters/NoteraEnemyCharacter.h"
#include "AbilitySystem/NoteraAbilitySystemComponent.h"
#include "AbilitySystem/NoteraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameplayTags/NoteraTags.h"
#include "Net/UnrealNetwork.h"
#include "Utils/DebugUtil.h"

ANoteraEnemyCharacter::ANoteraEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create ASC
	AbilitySystemComponent = CreateDefaultSubobject<UNoteraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Create AttributeSet
	AttributeSet = CreateDefaultSubobject<UNoteraAttributeSet>("AttributeSet");
}

void ANoteraEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* ANoteraEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ANoteraEnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void ANoteraEnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	PRINT_DEBUG("Enemy launched");

	AAIController* AIController = GetController<AAIController>();
	if(!IsValid(AIController))
	{
		PRINT_DEBUG_WARNING("StopMovementUntilLanded failed: No AIController");
		return;
	}

	AIController->StopMovement();

	if(!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
}

void ANoteraEnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	PRINT_DEBUG("Enemy landed");

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, NoteraTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}

void ANoteraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(!IsValid(GetAbilitySystemComponent()))
	{
		PRINT_DEBUG_WARNING("ASC invalid in BeginPlay");
		return;
	}

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnAscInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if(!HasAuthority())
	{
		PRINT_DEBUG("Client-side BeginPlay complete");
		return;
	}

	GiveStartupAbilities();
	InitializeAttributes();

	const UNoteraAttributeSet* NoteraAttributeSet = Cast<UNoteraAttributeSet>(GetAttributeSet());
	if(!IsValid(NoteraAttributeSet))
	{
		PRINT_DEBUG_WARNING("AttributeSet invalid during health binding");
		return;
	}

	const FGameplayAttribute HealthAttribute = NoteraAttributeSet->GetHealthAttribute();
	FOnGameplayAttributeValueChange HealthChangeDelegate = GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(HealthAttribute);
	HealthChangeDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
}

void ANoteraEnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	PRINT_DEBUG("Enemy died, stopping movement");

	AAIController* AIController = GetController<AAIController>();
	if(!IsValid(AIController))
	{
		PRINT_DEBUG_WARNING("HandleDeath: No AIController");
		return;
	}

	AIController->StopMovement();
}
