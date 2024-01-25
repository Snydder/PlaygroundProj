// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "SlateCore.h"
#include "ShipHUDWidgets.generated.h"

UCLASS()
class PLAYGROUNDPROJECTV2_API UShipHUDWidgets : public UUserWidget
{
	GENERATED_BODY()
public:
	UShipHUDWidgets(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Updating ship specific widgets

	void UpdateCurrentFuel(float BoostFuelTank, float CurrentFuel, float MinBoostPercentRequired);

	void UpdateCurrentHandlingStats(float Velocity, float Acceleration, bool IsRegening, bool RAEnabled, FKey RotAssistKey = "");

	// Updating gamemode specific widgets
	void UpdateGamemodeStats(int32 CheckpointsTaken, int32 TotalCheckpoints, float TimeRemaining);

	// Switching the color of the time textblock when the gamemode runs out of time or if it's successful.
	void SwitchTimeColor(bool HasCompleted);

	// Ship specific widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTCurrentFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTIsRegening;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTRAEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* BoostProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* MinBoostProgressBar;

	// Gamemode specific widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTCheckpointsTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTRemainingTime;
};