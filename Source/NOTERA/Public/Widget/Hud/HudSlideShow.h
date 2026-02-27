// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/MediaSlide.h"
#include "HudSlideShow.generated.h"

class UImage;
class UMediaPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSldieShowFinished);

/**
 *
 */
UCLASS()
class NOTERA_API UHudSlideShow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMediaSlide> Slides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VideoVisibilityDelay = 0.05f;

	UFUNCTION(BlueprintCallable)
	void StartSlideshow();

	UFUNCTION(BlueprintCallable)
	void Skip();

	UFUNCTION()
	void MediaOpened(const FString Url);

	FOnSldieShowFinished OnSlideShowFinished;

	UFUNCTION()
	void NativeSlideShowFinished();

	UFUNCTION(BlueprintNativeEvent)
	void SlideShowFinished();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlideImage;

	/** Optional fade-in animation for slides starts from hidden goes to visible */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeInAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeInDuration = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeOutDuration = 0.1f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMediaPlayer> MediaPlayer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> VideoMaterial;

	virtual void NativeConstruct() override;
private:
	int32 CurrentIndex = 0;
	FTimerHandle SlideTimer;

	void ShowSlide(int32 Index);
	UFUNCTION()
	void NextSlide();
};
