// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMainController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

void AAIMainController::BeginPlay()
{
	Super::BeginPlay();

	// AI Behavior tree
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector("PlayerLocation", PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector("StartLocation", GetPawn()->GetActorLocation());
	}

}

void AAIMainController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

bool AAIMainController::IsDead() const
{
	AMainCharacter* ControlledCharacter = Cast<AMainCharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true;

}
