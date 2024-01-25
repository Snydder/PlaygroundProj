// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/HUD.h"
#include "ShipHUD.h"
#include "TimeAttackGameState.h"
#include "TimeAttackGamemode.h"
#include "GoldCheckpoint.generated.h"

class UNiagaraSystem;

UCLASS()
class PLAYGROUNDPROJECTV2_API AGoldCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoldCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Creating the properties for our checkpoint
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* GoldCheckpointVFX;

	// Creating functions to handle the overlap checks
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Creating a HUD variable
	AShipHUD* ShipHUD;

	// Creating the gamemode variable
	ATimeAttackGamemode* TAGM;
	// Creating the gamestate variable
	ATimeAttackGameState* TAGS;

};
