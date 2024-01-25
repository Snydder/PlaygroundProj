// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include <PlaygroundProjectV2/ShipHUDWidgets.h>
#include "ShipHUD.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDPROJECTV2_API AShipHUD : public AHUD
{
	GENERATED_BODY()

public: 
	AShipHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void UpdateCurrentFuel(float BoostFuelTank, float CurrentFuel, float MinBoostPercentRequired);

	UFUNCTION()
		void UpdateCurrentHandlingStats(float Velocity, float Acceleration, bool IsRegening, bool RAEnabled, FKey RotAssistKey = "");

	UFUNCTION()
		void UpdateGamemodeStats(int32 CheckpointsTaken, int32 TotalCheckpoints, float TimeRemaining);

	UFUNCTION()
		void SwitchTimeColor(bool hasCompleted);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> UShipHUDWidgetsClass;

private:
	UShipHUDWidgets* ShipHUDWidgets;

};
