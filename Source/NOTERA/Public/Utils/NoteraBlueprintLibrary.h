// Copyright. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NoteraBlueprintLibrary.generated.h"

struct FGameplayTag;
struct FGameplayEventData;
class UGameplayEffect;

// The cardinal directions, used for playing the hit reaction animation based on the direction of hit
UENUM(BlueprintType, Category = "Notera")
enum class EHitDirection : uint8
{
	Left,
	Right,
	Forward,
	Back
};

// Structure to hold the closest actor found.
USTRUCT(BlueprintType, Category = "Notera")
struct FClosestActorWithTagResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
};

/**
 * Utility Blueprint library for handling hit direction, damage events, knockback logic,
 * and actor search functionality used across the Notera Course project.
 */
UCLASS(Category = "Notera")
class NOTERA_API UNoteraBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Notera")
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);

	UFUNCTION(BlueprintPure, Category = "Notera")
	static FName GetHitDirectionName(const EHitDirection& HitDirection);

	UFUNCTION(BlueprintCallable, Category = "Notera", meta = (DefaultToSelf = "WorldContextObject"))
	static FClosestActorWithTagResult FindClosestActorWithTag(const UObject* WorldContextObject, const FVector& Origin, const FName& Tag, float SearchRange);

	UFUNCTION(BlueprintCallable, Category = "Notera")
	static void SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Notera")
	static void SendDamageEventToPlayers(TArray<AActor*> Targets, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Notera|Abilities")
	static TArray<AActor*> HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	static void DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius);

	UFUNCTION(BlueprintCallable, Category = "Notera|Abilities")
	static TArray<AActor*> ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius, float OuterRadius, float LaunchForceMagnitude, float RotationAngle = 45.f, bool bDrawDebugs = false);
};
