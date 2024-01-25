// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimeAttackGamemode.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDPROJECTV2_API ATimeAttackGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public: 
	ATimeAttackGamemode();

	virtual ~ATimeAttackGamemode() = default;

	/** Return the number of coins in the game */
	UFUNCTION(BlueprintPure, Category = "CheckpointCount")
		virtual int32 GetCurrentCheckpointCount() const;

	UFUNCTION(BlueprintCallable, Category = "CheckpointCount")
		virtual void SetCurrentCheckpointCount();

	UFUNCTION(BlueprintPure, Category = "CheckpointCount")
		virtual int32 GetTotalCheckpointCount() const;

	UFUNCTION(BlueprintCallable, Category = "CheckpointCount")
		virtual void SetTotalCheckpointCount(int32 toAdd);

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
