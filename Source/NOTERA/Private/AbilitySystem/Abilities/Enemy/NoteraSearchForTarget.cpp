// Copyright. All Rights Reserved.

#include "AbilitySystem/Abilities/Enemy/NoteraSearchForTarget.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTasks/NoteraWaitGameplayEvent.h"
#include "Characters/NoteraEnemyCharacter.h"
#include "GameplayTags/NoteraTags.h"
#include "NOTERA/NOTERA.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/NoteraBlueprintLibrary.h"
#include "Utils/DebugUtil.h"

UNoteraSearchForTarget::UNoteraSearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UNoteraSearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<ANoteraEnemyCharacter>(GetAvatarActorFromActorInfo());
	if(!OwningEnemy.IsValid())
	{
		PRINT_DEBUG_WARNING("Invalid Owning Enemy. OwningEnemy not set.");
		return;
	}

	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	if(!OwningAIController.IsValid())
	{
		PRINT_DEBUG_WARNING("Invalid AIController");
		return;
	}

	StartSearch();

	WaitGameplayEventTask = UNoteraWaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), NoteraTags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UNoteraSearchForTarget::StartSearch()
{
	PRINT_DEBUG("");

	if(!OwningEnemy.IsValid()) return;

	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);

	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}

void UNoteraSearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	PRINT_DEBUG("EndAttack Event Received");

	if(OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched) StartSearch();
}

void UNoteraSearchForTarget::Search()
{
	if(!OwningEnemy.IsValid()) return;

	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();

	const auto [ClosestActor, ClosestDistance] = UNoteraBlueprintLibrary::FindClosestActorWithTag(this, SearchOrigin, NoteraTags::Player, OwningEnemy->SearchRange);
	TargetBaseCharacter = Cast<ANoteraBaseCharacter>(ClosestActor);

	if(!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}

	PRINT_DEBUG("Target found: %s", *TargetBaseCharacter->GetName());

	if(TargetBaseCharacter->IsAlive()) MoveToTargetAndAttack();
	else StartSearch();
}

void UNoteraSearchForTarget::MoveToTargetAndAttack()
{
	if(!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;

	if(!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}

	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(OwningAIController.Get(), FVector(), TargetBaseCharacter.Get(), OwningEnemy->AcceptanceRadius);
	MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UNoteraSearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if(Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}

	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());

	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UNoteraSearchForTarget::Attack()
{
	if(!OwningEnemy.IsValid()) return;
	if(!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}

	PRINT_DEBUG("Attempting attack");

	// ReSharper disable once CppTooWideScopeInitStatement
	const FGameplayTag AttackTag = NoteraTags::NoteraAbilities::Enemy::Attack;

	if(!GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer()))
	{
		StartSearch();
		return;
	}
}
