// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGamemode.h"
#include "TimeAttackGameState.h"
#include "ShipHUD.h"
#include "DrawDebugHelpers.h"


ATimeAttackGamemode::ATimeAttackGamemode()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = ATimeAttackGameState::StaticClass();
	HUDClass = AShipHUD::StaticClass();
}

void ATimeAttackGamemode::BeginPlay()
{
	// Called when the game begins
}

void ATimeAttackGamemode::Tick(float DeltaSeconds)
{
	// Called every frame (disabled)
	Super::Tick(DeltaSeconds);
}


int32 ATimeAttackGamemode::GetCurrentCheckpointCount() const
{
	return GetGameState<ATimeAttackGameState>()->CurrentCheckpointCount;
}

void ATimeAttackGamemode::SetCurrentCheckpointCount()
{
	GetGameState<ATimeAttackGameState>()->CurrentCheckpointCount = GetCurrentCheckpointCount() + 1;
}

int32 ATimeAttackGamemode::GetTotalCheckpointCount() const
{
	return GetGameState<ATimeAttackGameState>()->TotalCheckpointCount;
}

void ATimeAttackGamemode::SetTotalCheckpointCount(int32 toAdd)
{
	GetGameState<ATimeAttackGameState>()->TotalCheckpointCount += toAdd;
}