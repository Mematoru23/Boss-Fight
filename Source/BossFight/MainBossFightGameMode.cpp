// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBossFightGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "AIMainController.h"

void AMainBossFightGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	// For loop over all AIEnemies in world
	for (AAIMainController* Controller : TActorRange<AAIMainController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return;
		}
	}
	EndGame(true);
}

// Ending the game, win/lose
void AMainBossFightGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
