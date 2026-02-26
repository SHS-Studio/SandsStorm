//

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimBaseChar.generated.h"

class ABaseCharacter;

/**
 *
 */
UCLASS()
class NOTERA_API UAnimBaseChar : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABaseCharacter* BaseCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCrouching;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
