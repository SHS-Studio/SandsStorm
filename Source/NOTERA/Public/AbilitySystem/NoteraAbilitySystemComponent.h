// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NoteraAbilitySystemComponent.generated.h"

/**
 * Custom Ability System Component for handling auto-activation, leveling, and replicated ability setup
 */
UCLASS(ClassGroup=(Custom), Category = "Notera", meta=(BlueprintSpawnableComponent))
class NOTERA_API UNoteraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(BlueprintCallable, Category = "Notera|Abilities")
	void SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1);

	UFUNCTION(BlueprintCallable, Category = "Notera|Abilities")
	void AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 LevelsToAdd = 1);

private:
	void HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec);
};
