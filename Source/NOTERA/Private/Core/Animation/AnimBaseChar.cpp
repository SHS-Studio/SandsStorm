//


#include "AnimBaseChar.h"
#include "Core/Characters/BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void UAnimBaseChar::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UAnimBaseChar::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if(!IsValid(BaseCharacter))
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UAnimBaseChar::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!IsValid(BaseCharacter))
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
		return;
	}

	Speed = BaseCharacter->GetVelocity().Size();
	bIsInAir = BaseCharacter->GetMovementComponent()->IsFalling();
	bIsDead = BaseCharacter->Health <= 0;
	bIsCrouching = BaseCharacter->bIsCrouched;
}
