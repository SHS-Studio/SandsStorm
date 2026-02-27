// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Hud/HudSlideShow.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"

void UHudSlideShow::StartSlideshow()
{
	if(IsValid(SlideImage))
		SlideImage->SetVisibility(ESlateVisibility::Hidden);

	CurrentIndex = 0;
	ShowSlide(CurrentIndex);
}

void UHudSlideShow::Skip()
{
	NextSlide();
}

// ReSharper disable once CppPassValueParameterByConstReference
// ReSharper disable once CppMemberFunctionMayBeConst
void UHudSlideShow::MediaOpened(const FString Url)
{
	FTimerHandle VisibilityTimer;
	GetWorld()->GetTimerManager().SetTimer( VisibilityTimer, [this]() {
			SlideImage->SetVisibility(ESlateVisibility::Visible);
		}, VideoVisibilityDelay, false);
}

void UHudSlideShow::NativeSlideShowFinished()
{

	SlideShowFinished();
}

void UHudSlideShow::SlideShowFinished_Implementation()
{
}

void UHudSlideShow::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(MediaPlayer))
	{
		MediaPlayer->OnMediaOpened.AddDynamic(this, &ThisClass::MediaOpened);
		MediaPlayer->OnEndReached.AddDynamic(this, &ThisClass::NextSlide);
	}

	OnSlideShowFinished.AddDynamic(this, &ThisClass::NativeSlideShowFinished);

	StartSlideshow();
}

void UHudSlideShow::ShowSlide(const int32 Index)
{
	if (!Slides.IsValidIndex(Index)) return;

	GetWorld()->GetTimerManager().ClearTimer(SlideTimer);
	const auto& [Image, ImageDuration, MediaSource] = Slides[Index];

	const FString DataName = Image ? Image->GetName() : MediaSource ? MediaSource->GetName() : TEXT("Invalid Slide");
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Showing slide %d with data %s"), Index, *DataName));

	if (IsValid(Image))
	{
		SlideImage->SetBrushFromTexture(Image);

		GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ThisClass::NextSlide, ImageDuration, false);
	}
	else if (IsValid(MediaSource))
	{
		SlideImage->SetBrushFromMaterial(VideoMaterial);
		MediaPlayer->OpenSource(MediaSource);
	}
	else
		return;

	SlideImage->SetVisibility(ESlateVisibility::Visible);

	PlayAnimationForward(FadeInAnim, 1.f / FadeInDuration);
}

void UHudSlideShow::NextSlide()
{
	if(IsValid(FadeInAnim))
		PlayAnimationReverse(FadeInAnim, 1.f/ FadeOutDuration);

	FTimerHandle Unused;
	GetWorld()->GetTimerManager().SetTimer(Unused, [this]() {
		CurrentIndex++;
		if (Slides.IsValidIndex(CurrentIndex))
			ShowSlide(CurrentIndex);
		else
			OnSlideShowFinished.Broadcast();
	}, FadeOutDuration, false);
}
