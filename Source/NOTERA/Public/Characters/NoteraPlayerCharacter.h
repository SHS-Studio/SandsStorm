// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NoteraBaseCharacter.h"
#include "NoteraPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * Player character class for the Notera course.
 */
UCLASS(Category = "Notera")
class NOTERA_API ANoteraPlayerCharacter : public ANoteraBaseCharacter
{
	GENERATED_BODY()

public:
	ANoteraPlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual UAttributeSet* GetAttributeSet() const override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Notera|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Notera|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};
