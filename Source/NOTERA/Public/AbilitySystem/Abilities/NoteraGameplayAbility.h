// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NoteraGameplayAbility.generated.h"

/**
 * Base Gameplay Ability class for Notera Course.
 * Adds optional debug visualization.
 */
UCLASS(Category = "Notera")
class NOTERA_API UNoteraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Enables debug for the ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Notera|Debug")
	bool bDrawDebugs = false;

	// Called when the ability is activated
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Called when the ability ends (normally or via cancellation)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
