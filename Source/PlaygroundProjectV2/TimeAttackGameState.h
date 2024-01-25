// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShipHUD.h"
#include "TimeAttackGameState.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDPROJECTV2_API ATimeAttackGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATimeAttackGameState();
	virtual ~ATimeAttackGameState() = default;

	UPROPERTY(EditAnywhere, Category = "Time")
		float TimeAttackMax = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Time")
		float TimeIncrement = 0.0f;
	
	// Getting and returning the remaining timer
	UFUNCTION()
		float GetRemainingTime(bool read = false);

	UFUNCTION()
		void UpdateShipHUD();

	UFUNCTION()
		void TimerFunction();

	UFUNCTION()
		void IncrementResetTimer();

	//Timer handler variable
	FTimerHandle TimerHandle;

	// How many checkpoints were taken
	int32 CurrentCheckpointCount = 0.0f;

	// How many checkpoints are there in total
	int32 TotalCheckpointCount = 0.0f;

	float TimeTracker = 0.0f;

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float RemainingTime = 0.0f;
	int32 TimerRemainingExecAmount = 1;

	// Creating the HUD variable
	AShipHUD* ShipHUD;
};
