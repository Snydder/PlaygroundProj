// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TimeAttackGamemode.h"
#include "TimeAttackGameState.h"
#include "RazorbackPawn.generated.h"

class UNiagaraSystem;
class USound;

UCLASS()
class PLAYGROUNDPROJECTV2_API ARazorbackPawn : public APawn
{
	GENERATED_BODY()

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Creating the mesh and the camera

	UPROPERTY(EditAnywhere, Category = "ActorComponents")
		class UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "ActorComponents")
		class UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, Category = "ActorComponents")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "ActorComponents")
		class USpringArmComponent* CameraSpringArm;

	// Flight characteristics

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float FwdAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float BwdAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float YAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float ZAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float RollAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float RotAccel;

	UPROPERTY(EditAnywhere, Category = "FlightTuning")
		float MassScale;

	UPROPERTY(EditAnywhere, Category = "Dampening")
		float AngularDamping;

	UPROPERTY(EditAnywhere, Category = "Dampening")
		float LinearDamping;

	UPROPERTY(EditAnywhere, Category = "Dampening")
		float LinearDampingSpeedThreshold;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float TranslationBoostIncrement;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float RotationBoostIncrement;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float BoostFuelTank;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float BoostConsumptionRate;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float BoostRecoveryRate;

	UPROPERTY(EditAnywhere, Category = "Boost")
		float RotationAssistRotRateBoostThresholdConsume;

	UPROPERTY(EditAnywhere, Category = "Boost", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
		float MinBoostPercentRequired;

	UPROPERTY(EditAnywhere, Category = "Camera")
		bool EnableCameraLag;

	UPROPERTY(EditAnywhere, Category = "Camera")
		bool EnableCameraRotationLag;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float CameraLagSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float CameraRotationLagSpeed;

	// Creating the Arrays that will store the information that tells wich Axis the thrusters are supposed to belong.
	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> TopThrustersArray;

	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> BottomThrustersArray;

	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> LeftThrustersArray;

	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> RightThrustersArray;

	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> MainThrustersArray;

	UPROPERTY(EditAnywhere, Category = "TranslationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> RetroThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RollThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> LeftRollThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RollThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> RightRollThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RotationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> LeftRotThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RotationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> RightRotThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RotationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> BackRotThrustersArray;

	UPROPERTY(EditAnywhere, Category = "RotationThrusters", meta = (UseComponentPicker, AllowAnyActor))
		TArray <FComponentReference> FrontRotThrustersArray;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* NS_Thruster;

	UPROPERTY(EditAnywhere, Category = "SFX")
		USoundBase* SB_Thruster;


	// Adding the flight controller component to the ship
	UPROPERTY(VisibleAnywhere)
	class USFCS* SFCS;

private:

	// Translational Movement functions
	UFUNCTION()
	void XThrust(float AxisValue);
	UFUNCTION()
	void YThrust(float AxisValue);
	UFUNCTION()
	void ZThrust(float AxisValue);

	UFUNCTION()
	void XRotation(float AxisValue);
	UFUNCTION()
	void YRotation(float AxisValue);
	UFUNCTION()
	void ZRotation(float AxisValue);

	// Braking translation and rotation separatedly
	UFUNCTION()
	void TranslationBrake(float AxisValue);
	UFUNCTION()
	void RotationBrake(float AxisValue);

	// Acceleration multiplier
	UFUNCTION()
	void Boost(float AxisValue);

	// Rotation Assist toggle
	UFUNCTION()
	void RotationAssistToggle(FKey tempFKey);

	// Scan for empty thruster arrays
	UFUNCTION()
	bool ThrusterArraySweep();

	UFUNCTION()
	void RestartGamemode();

	// Array flags
	bool TopThrArrayFilled = true;
	bool BottomThrArrayFilled = true;
	bool LeftThrArrayFilled = true;
	bool RightThrArrayFilled = true;
	bool MainThrArrayFilled = true;
	bool RetroThrArrayFilled = true;
	bool LeftRollThrustersArrayFilled = true;
	bool RightRollThrustersArrayFilled = true;
	bool LeftRotThrustersArrayFilled = true;
	bool RightRotThrustersArrayFilled = true;
	bool BackRotThrustersArrayFilled = true;
	bool FrontRotThrustersArrayFilled = true;

	// Input variables
	FVector CurrentVelocity;

	// Axis Variables naming 
	FName NameStrafeForward = "StrafeFwd";
	FName NameStrafeBackwards = "StrafeBwd";
	FName NameStrafeLeft = "StrafeLft";
	FName NameStrafeRight = "StrafeRgt";
	FName NameStrafeUp = "StrafeUp";
	FName NameStrafeDown = "StrafeDwn";
	FName NameRollLeft = "RollLft";
	FName NameRollRight = "RollRgt";
	FName NameRotLeft = "RotLft";
	FName NameRotRight = "RotRgt";
	FName NameRotUp = "RotUp";
	FName NameRotDown = "RotDwn";
	FName NameXStrafe = "XStrafe";
	FName NameYStrafe = "YStrafe";
	FName NameZStrafe = "ZStrafe";
	FName NameXRotation = "XRotation";
	FName NameYRotation = "YRotation";
	FName NameZRotation = "ZRotation";
	FName TranslatBrk = "TranslatBrk";
	FName RotBrk = "BrkRttn";
	FKey RotAssistKey = "";

	// Action Variables naming
	FName NameRotationAssistMode = "RotationAssist";
	FName NameRestartGame = "Restart";

	// Getting a reference to "self"
	AActor* Self;

	// Creating the gamemode variable
	ATimeAttackGamemode* TAGM;

	// Sets default values for this pawn's properties
	ARazorbackPawn();
};
