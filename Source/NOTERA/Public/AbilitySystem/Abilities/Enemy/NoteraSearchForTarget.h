// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NoteraGameplayAbility.h"
#include "NoteraSearchForTarget.generated.h"

namespace EPathFollowingResult
{
	enum Type : int;
}

class UAITask_MoveTo;
class ANoteraBaseCharacter;
class UAbilityTask_WaitDelay;
class ANoteraEnemyCharacter;
class AAIController;
class UNoteraWaitGameplayEvent;

/**
 * Ability used by Npcs to search, move toward, and attack nearby valid targets(player)
 */
UCLASS(Category = "Notera")
class NOTERA_API UNoteraSearchForTarget : public UNoteraGameplayAbility
{
	GENERATED_BODY()

public:
	UNoteraSearchForTarget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TWeakObjectPtr<ANoteraEnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<ANoteraBaseCharacter> TargetBaseCharacter;

private:
	UPROPERTY()
	TObjectPtr<UNoteraWaitGameplayEvent> WaitGameplayEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;

	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;

	void StartSearch();

	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void Search();

	void MoveToTargetAndAttack();

	UFUNCTION()
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);

	UFUNCTION()
	void Attack();
};
