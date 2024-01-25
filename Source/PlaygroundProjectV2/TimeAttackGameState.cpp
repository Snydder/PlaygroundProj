// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameState.h"
#include "DrawDebugHelpers.h"

ATimeAttackGameState::ATimeAttackGameState() : CurrentCheckpointCount(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeAttackGameState::BeginPlay()
{
	// Called when the game begins
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimeAttackGameState::TimerFunction, TimeAttackMax, true, 0.0f);
	ShipHUD = Cast<AShipHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void ATimeAttackGameState::Tick(float DeltaSeconds)
{
	// Called every frame
	Super::Tick(DeltaSeconds);

	// Get the remaining time
	GetRemainingTime();

	// If ShipHUD is valid, update the hud 
	if (ShipHUD)
		UpdateShipHUD();
}

void ATimeAttackGameState::TimerFunction()
{
	if (TimerRemainingExecAmount == 0 || CurrentCheckpointCount == TotalCheckpointCount)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	TimerRemainingExecAmount--;
}

float ATimeAttackGameState::GetRemainingTime(bool read)
{
	RemainingTime = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
	if (read) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Time Remaining: %.2f"), RemainingTime));
	}
	return RemainingTime;
}

void ATimeAttackGameState::UpdateShipHUD()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))
	ShipHUD->UpdateGamemodeStats(CurrentCheckpointCount, TotalCheckpointCount, GetRemainingTime());
	else 
		ShipHUD->UpdateGamemodeStats(CurrentCheckpointCount, TotalCheckpointCount, TimeTracker);

	if (!(GetWorldTimerManager().IsTimerActive(TimerHandle)) && CurrentCheckpointCount < TotalCheckpointCount) 	// Checking if the timer is paused and not all checkpoints have been taken
		ShipHUD->SwitchTimeColor(false);

	else if ((GetWorldTimerManager().IsTimerActive(TimerHandle)) && CurrentCheckpointCount == TotalCheckpointCount)	// Checking if the timer is still active and all checkpoints have been captured.
	{
		ShipHUD->SwitchTimeColor(true);
		GetWorldTimerManager().PauseTimer(TimerHandle);
	}
}

void ATimeAttackGameState::IncrementResetTimer()
{
	float tempRemainingTime = GetRemainingTime();
	float tempFinalTime = tempRemainingTime + TimeIncrement;

	tempFinalTime = FMath::Clamp(tempFinalTime, 0.0f, TimeAttackMax);

	//GetWorldTimerManager().ClearTimer(TimerHandle);
	TimerRemainingExecAmount = 1;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimeAttackGameState::TimerFunction, tempFinalTime, true, 0.0f);
}