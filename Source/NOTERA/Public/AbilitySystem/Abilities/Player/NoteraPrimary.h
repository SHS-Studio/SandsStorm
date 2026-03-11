// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NoteraGameplayAbility.h"
#include "NoteraPrimary.generated.h"

/**
 * Player's primary attack game ability
 */
UCLASS(Category = "Notera")
class NOTERA_API UNoteraPrimary : public UNoteraGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Notera|Abilities")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	void SendHitReactEventToActors(const TArray<AActor*>& ActorsHit) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Notera|Abilities")
	float HitBoxRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Notera|Abilities")
	float HitBoxForwardOffset = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Notera|Abilities")
	float HitBoxElevationOffset = 20.0f;
};
