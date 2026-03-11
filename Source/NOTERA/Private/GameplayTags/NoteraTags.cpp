// Copyright. All Rights Reserved.

#include "GameplayTags/NoteraTags.h"

namespace NoteraTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "NoteraTags.None", "None")

	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "NoteraTags.SetByCaller.Projectile", "Tag for Set by Caller Magnitude for Projectiles")
	}

	namespace NoteraAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "NoteraTags.NoteraAbilities.ActivateOnGiven", "Tag for Abilities that should activate immediately once given.")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "NoteraTags.NoteraAbilities.Primary", "Tag for the Primary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "NoteraTags.NoteraAbilities.Secondary", "Tag for the Secondary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "NoteraTags.NoteraAbilities.Tertiary", "Tag for the Tertiary Ability")

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "NoteraTags.NoteraAbilities.Enemy.Attack", "Enemy Attack Tag")
		}
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "NoteraTags.Events.KillScored", "Tag for the KillScored Event")

		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "NoteraTags.Events.Player.HitReact", "Tag for the Player HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "NoteraTags.Events.Player.Death", "Tag for the Player Death Event")
		}

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "NoteraTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "NoteraTags.Events.Enemy.EndAttack", "Tag for the Enemy Ending an Attack")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit, "NoteraTags.Events.Enemy.MeleeTraceHit", "Tag for the Enemy Melee Trace Hit")
		}
	}
}
