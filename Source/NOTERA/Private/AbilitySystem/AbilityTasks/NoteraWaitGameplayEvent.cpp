// Copyright. All Rights Reserved.


#include "AbilitySystem/AbilityTasks/NoteraWaitGameplayEvent.h"

UNoteraWaitGameplayEvent* UNoteraWaitGameplayEvent::WaitGameplayEventToActorProxy(AActor* TargetActor, const FGameplayTag EventTag, const bool OnlyTriggerOnce, const bool OnlyMatchExact)
{
	UNoteraWaitGameplayEvent* MyObj = NewObject<UNoteraWaitGameplayEvent>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}

void UNoteraWaitGameplayEvent::StartActivation()
{
	Activate();
}
