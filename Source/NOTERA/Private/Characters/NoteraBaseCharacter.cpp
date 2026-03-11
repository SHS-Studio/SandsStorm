// Copyright. All Rights Reserved.

#include "Characters/NoteraBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/NoteraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Utils/DebugUtil.h"

ANoteraBaseCharacter::ANoteraBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Make hitboxes work on server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ANoteraBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bAlive);
}

UAbilitySystemComponent* ANoteraBaseCharacter::GetAbilitySystemComponent() const
{
	// Override in child classes
	return nullptr;
}

void ANoteraBaseCharacter::GiveStartupAbilities()
{
	if(!IsValid(GetAbilitySystemComponent()))
	{
		PRINT_DEBUG_WARNING("ASC invalid. Cannot give startup abilities");
		return;
	}

	for(const auto& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}

	PRINT_DEBUG("Startup abilities granted");
}

void ANoteraBaseCharacter::InitializeAttributes() const
{
	if(!IsValid(InitializeAttributesEffect))
	{
		PRINT_DEBUG_WARNING("InitializeAttributesEffect missing");
		return;
	}

	if(!IsValid(GetAbilitySystemComponent()))
	{
		PRINT_DEBUG_WARNING("ASC missing during InitializeAttributes");
		return;
	}

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributesEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	PRINT_DEBUG("Initial attributes applied");

	if(UNoteraAttributeSet* AttributeSet = Cast<UNoteraAttributeSet>(GetAttributeSet()))
		AttributeSet->PostAttributeInitialized();
}

void ANoteraBaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	PRINT_DEBUG("Health changed %f", AttributeChangeData.NewValue);

	if(AttributeChangeData.NewValue <= 0.f)
		HandleDeath();
}

void ANoteraBaseCharacter::HandleDeath()
{
	bAlive = false;
	PRINT_DEBUG("%s has died", *GetName());
}

void ANoteraBaseCharacter::HandleRespawn()
{
	bAlive = true;
	PRINT_DEBUG("Character respawned");
}

void ANoteraBaseCharacter::ResetAttributes() const
{
	if(!IsValid(ResetAttributesEffect))
	{
		PRINT_DEBUG_WARNING("ResetAttributesEffect missing");
		return;
	}

	if(!IsValid(GetAbilitySystemComponent()))
	{
		PRINT_DEBUG_WARNING("ASC missing during ResetAttributes");
		return;
	}

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(ResetAttributesEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	PRINT_DEBUG("Attributes reset");
}
