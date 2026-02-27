#pragma once

#include "MediaSlide.generated.h"

class UMediaSource;

USTRUCT(BlueprintType)
struct FMediaSlide
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Image = nullptr;

	/** Duration for which the image will be shown, excluding the fade times at both ends */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImageDuration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMediaSource> MediaSource = nullptr;
};
