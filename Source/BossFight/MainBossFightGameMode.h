// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossFightGameModeBase.h"
#include "MainBossFightGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BOSSFIGHT_API AMainBossFightGameMode : public ABossFightGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
	
};
