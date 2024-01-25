// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipHUDWidgets.h"

UShipHUDWidgets::UShipHUDWidgets(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UShipHUDWidgets::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShipHUDWidgets::UpdateCurrentFuel(float BoostFuelTank, float CurrentFuel, float MinBoostPercentRequired)
{
	float CurrentFuelToPercent = CurrentFuel / BoostFuelTank;
	float tempMinBoostPercentRequired = MinBoostPercentRequired / 100;
	FColor AboveLimitFuelColor = FColor::FromHex("#a3d3ff");
	FColor BelowLimitFuelColor = FColor::FromHex("#ff0000");

	if (TXTCurrentFuel)
	{
		// Turn textBox visible if hidden
		if (TXTCurrentFuel->Visibility == ESlateVisibility::Hidden)
		{
			TXTCurrentFuel->SetVisibility(ESlateVisibility::Visible);
		}
		TXTCurrentFuel->SetText(FText::FromString("Fuel: " + FString::FromInt(CurrentFuelToPercent * 100) + "%"));
	}

	if (BoostProgressBar)
	{
		// Turn textBox visible if hidden
		if (BoostProgressBar->Visibility == ESlateVisibility::Hidden)
		{
			BoostProgressBar->SetVisibility(ESlateVisibility::Visible);
		}
		BoostProgressBar->SetPercent(CurrentFuelToPercent);
		if (CurrentFuelToPercent > tempMinBoostPercentRequired)
			BoostProgressBar->SetFillColorAndOpacity(FLinearColor::FromSRGBColor(AboveLimitFuelColor));
		else
			BoostProgressBar->SetFillColorAndOpacity(FLinearColor::FromSRGBColor(BelowLimitFuelColor));
	}
	if (MinBoostProgressBar)
	{
		// Turn textBox visible if hidden
		if (MinBoostProgressBar->Visibility == ESlateVisibility::Hidden)
		{
			MinBoostProgressBar->SetVisibility(ESlateVisibility::Visible);
		}
		MinBoostProgressBar->SetPercent(tempMinBoostPercentRequired);
		MinBoostProgressBar->SetFillColorAndOpacity(FLinearColor::FromSRGBColor(BelowLimitFuelColor));
		if (CurrentFuelToPercent > tempMinBoostPercentRequired)
		{
			MinBoostProgressBar->SetRenderOpacity(1.0f);
		}
		else
		{
			MinBoostProgressBar->SetRenderOpacity(0.5f);
		}
	}
}

// Update the handling stats such as velocity, accel, etc
void UShipHUDWidgets::UpdateCurrentHandlingStats(float Velocity, float Acceleration, bool IsRegening, bool RAEnabled, FKey RotAssistKey)
{
	int32 tempIntVelocity = Velocity;
	int32 tempIntAcceleration = FMath::Abs(Acceleration);

	// Velocity
	if (TXTVelocity)
	{
		// Turn textBox visible if hidden
		if (TXTVelocity->Visibility == ESlateVisibility::Hidden)
		{
			TXTVelocity->SetVisibility(ESlateVisibility::Visible);
		}
		TXTVelocity->SetText(FText::FromString("Velocity: " + FString::FromInt(tempIntVelocity) + "m/s"));
	}

	// Acceleration
	if (TXTAcceleration)
	{
		// Turn textBox visible if hidden
		if (TXTAcceleration->Visibility == ESlateVisibility::Hidden)
		{
			TXTAcceleration->SetVisibility(ESlateVisibility::Visible);
		}
		TXTAcceleration->SetText(FText::FromString("Acceleration: " + FString::FromInt(tempIntAcceleration) + "m/s"));
	}

	// Is regening boost

	if (TXTIsRegening)
	{
		if (IsRegening)
		{
			// Turn textBox visible if hidden
			if (TXTIsRegening->Visibility == ESlateVisibility::Hidden)
			{
				TXTIsRegening->SetVisibility(ESlateVisibility::Visible);
				TXTIsRegening->SetText(FText::FromString("Regening"));
			}
		}
		else
		{
			// Turn textBox visible if hidden
			if (TXTIsRegening->Visibility == ESlateVisibility::Visible)
			{
				TXTIsRegening->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	// Is Rotation Assist
	// Acceleration
	if (TXTRAEnabled)
	{
		// Turn textBox visible if hidden
		if (TXTRAEnabled->Visibility == ESlateVisibility::Hidden)
		{
			TXTRAEnabled->SetVisibility(ESlateVisibility::Visible);
		}
		(RAEnabled) ? TXTRAEnabled->SetText(FText::FromString("[" + RotAssistKey.GetFName().ToString() + "]" + "Rotation assist: Active")) : TXTRAEnabled->SetText(FText::FromString("[" + RotAssistKey.GetFName().ToString() + "]" + "Rotation assist: Inactive"));
	}
}

// Updating the gamemode widgets
void UShipHUDWidgets::UpdateGamemodeStats(int32 CheckpointsTaken, int32 TotalCheckpoints, float RemainingTime)
{
	int32 tempRemainingTime = RemainingTime;

	if (TXTCheckpointsTaken)
	{
		// Turn textBox visible if hidden
		if (TXTCheckpointsTaken->Visibility == ESlateVisibility::Hidden)
		{
			TXTCheckpointsTaken->SetVisibility(ESlateVisibility::Visible);
		}
		TXTCheckpointsTaken->SetText(FText::FromString(FString::Printf(TEXT("Checkpoints Taken: %d \\ %d"), CheckpointsTaken, TotalCheckpoints)));
	}

	if (TXTRemainingTime)
	{
		// Turn textBox visible if hidden
		if (TXTRemainingTime->Visibility == ESlateVisibility::Hidden)
		{
			TXTRemainingTime->SetVisibility(ESlateVisibility::Visible);
		}
		if (tempRemainingTime < 0)
		{
			tempRemainingTime = 0;
		}
		TXTRemainingTime->SetText(FText::FromString(FString::Printf(TEXT("Remaining time: %.2f"), RemainingTime)));
	}
}

void UShipHUDWidgets::SwitchTimeColor(bool HasCompleted)
{
	FColor SuccessColor = FColor::FromHex("#408fff");
	FColor FailColor = FColor::FromHex("#b30000");
	if (TXTRemainingTime)
	{
		// Turn textBox visible if hidden
		if (TXTRemainingTime->Visibility == ESlateVisibility::Hidden)
		{
			TXTRemainingTime->SetVisibility(ESlateVisibility::Visible);
		}
		// If time is bigger than zero, set to blue. If not, set to red.
		if (HasCompleted)
			TXTRemainingTime->SetColorAndOpacity(FLinearColor::FromSRGBColor(SuccessColor));
		else 
			TXTRemainingTime->SetColorAndOpacity(FLinearColor::FromSRGBColor(FailColor));
	}
}