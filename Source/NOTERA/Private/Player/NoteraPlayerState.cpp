// Copyright. All Rights Reserved.


#include "Player/NoteraPlayerState.h"
#include "AbilitySystem/NoteraAbilitySystemComponent.h"
#include "AbilitySystem/NoteraAttributeSet.h"

ANoteraPlayerState::ANoteraPlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UNoteraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UNoteraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ANoteraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}