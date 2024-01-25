// Fill out your copyright notice in the Description page of Project Settings.
#include "ShipHUD.h"

AShipHUD::AShipHUD()
{

}

void AShipHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UShipHUDWidgetsClass)
	{
		ShipHUDWidgets = CreateWidget<UShipHUDWidgets>(GetWorld(), UShipHUDWidgetsClass);
		if (ShipHUDWidgets)
		{
			ShipHUDWidgets->AddToViewport();
		}
	}
}

void AShipHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AShipHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AShipHUD::UpdateCurrentFuel(float BoostFuelTank, float CurrentFuel, float MinBoostPercentRequired)
{
	if (ShipHUDWidgets)
	{
		ShipHUDWidgets->UpdateCurrentFuel(BoostFuelTank, CurrentFuel, MinBoostPercentRequired);
	}
}

void AShipHUD::UpdateCurrentHandlingStats(float Velocity, float Acceleration, bool IsRegening, bool RAEnabled, FKey RotAssistKey)
{
	if (ShipHUDWidgets)
	{
		ShipHUDWidgets->UpdateCurrentHandlingStats(Velocity, Acceleration, IsRegening, RAEnabled, RotAssistKey);
	}
}

void AShipHUD::UpdateGamemodeStats(int32 CheckpointsTaken, int32 TotalCheckpoints, float TimeRemaining)
{
	if (ShipHUDWidgets)
	{
		ShipHUDWidgets->UpdateGamemodeStats(CheckpointsTaken, TotalCheckpoints, TimeRemaining);
	}
}

void AShipHUD::SwitchTimeColor(bool hasCompleted)
{
	if (ShipHUDWidgets)
	{
		ShipHUDWidgets->SwitchTimeColor(hasCompleted);
	}
}