// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFightController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void ABossFightController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void ABossFightController::GameHasEnded(class AActor* EndGameFocus /*= nullptr*/, bool bIsWinner /*= false*/)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTime, this, &APlayerController::RestartLevel, RestartDelay);

}
