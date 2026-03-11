// Copyright. All Rights Reserved.


#include "AbilitySystem/Abilities/Player/NoteraPrimary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/NoteraTags.h"

void UNoteraPrimary::SendHitReactEventToActors(const TArray<AActor*>& ActorsHit) const
{
	for(AActor* HitActor : ActorsHit)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, NoteraTags::Events::Enemy::HitReact, Payload);
	}
}
