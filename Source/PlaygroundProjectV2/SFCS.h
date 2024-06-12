// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "RazorbackPawn.h"
#include "GameFramework/HUD.h"
#include "ShipHUD.h"
#include "SFCS.generated.h"

class UNiagaraSystem;
class USound;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYGROUNDPROJECTV2_API USFCS : public UActorComponent
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Sets default values for this component's properties
	USFCS();

	//Thruster Arrays
	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> MainThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> RetroThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> TopThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> BottomThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> LeftThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "TranslationThrusters")
		TArray <UPhysicsThrusterComponent*> RightThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RollThrusters")
		TArray <UPhysicsThrusterComponent*> LeftRollThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RollThrusters")
		TArray <UPhysicsThrusterComponent*> RightRollThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RotationThrusters")
		TArray <UPhysicsThrusterComponent*> LeftRotThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RotationThrusters")
		TArray <UPhysicsThrusterComponent*> RightRotThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RotationThrusters")
		TArray <UPhysicsThrusterComponent*> BackRotThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "RotationThrusters")
		TArray <UPhysicsThrusterComponent*> FrontRotThrusters;

	UPROPERTY(BlueprintReadOnly, Category = "VFX")
		UNiagaraSystem* NS_Thruster;

	UPROPERTY(BlueprintReadOnly, Category = "VFX")
		USoundBase* SB_Thruster;

	UPROPERTY(BlueprintReadOnly, Category = "VFX")
		UNiagaraSystem* CheckpointCaptureExplosion;

	// Thruster intializer
	UFUNCTION()
		TArray <UPhysicsThrusterComponent*> ThrusterInitializer(TArray <FComponentReference> ThrusterRef, TArray <UPhysicsThrusterComponent*> Thruster);

public:

	// --------------------------------------------------------- Functions ---------------------------------------------------------  \\

	// Getting owner cube mesh
	UStaticMeshComponent* CubeOwnerStaticMesh;
	UFUNCTION()
	void SetCubeOwnerStaticMesh(UStaticMeshComponent* CubeMesh)
	{
		CubeOwnerStaticMesh = CubeMesh;
	}

	// Getting owner visual mesh
	UStaticMeshComponent* VisualOwnerStaticMesh;
	UFUNCTION()
	void SetVisualOwnerStaticMesh(UStaticMeshComponent* VisualMesh)
	{
		VisualOwnerStaticMesh = VisualMesh;
	}

	// Getting all thrusters on SFCS system from the ship (by reference)
	UFUNCTION()
	void GetXThrusters(
		TArray <FComponentReference> MainThrustersRef,
		TArray <FComponentReference> RetroThrustersRef);

	UFUNCTION()
	void GetYThrusters(
		TArray <FComponentReference> LeftThrustersRef,
		TArray <FComponentReference> RightThrustersRef);

	UFUNCTION()
	void GetZThrusters(
		TArray <FComponentReference> TopThrustersRef,
		TArray <FComponentReference> BottomThrustersRef);

	UFUNCTION()
	void GetRotThrusters(
		TArray <FComponentReference> LeftRotThrusterRef,
		TArray <FComponentReference> RightRotThrusterRef,
		TArray <FComponentReference> BackRotThrusterRef,
		TArray <FComponentReference> FrontRotThrusterRef);

	UFUNCTION()
	void GetRollThrusters(
		TArray <FComponentReference> LeftRollThrustersRef,
		TArray <FComponentReference> RightRollThrustersRef);

	// Getting flight handling characterisctics
	UFUNCTION()
	void GetHandlingParams(
		float TempFwdAccel,
		float TempBwdAccel,
		float TempYAccel,
		float TempZAccel,
		float TempRollAccel,
		float TempRotAccel,
		float TempAngularDamping,
		float TempTranslationBoostIncrement,
		float TempRotationBoostIncrement,
		float TempBoostFuelTank, 
		float TempBoostConsumptionRate,
		float TempBoostRecoveryRate,
		float TempMinBoostPercentRequired,
		float TempRARotRateBoostThresholdConsume,
		float TempMass,
		float TempLinearDamping,
		float TempLinearDampingSpeedThreshold);

	// Getting keybinding button label variable
	UFUNCTION()
		void SetBindingKeys(FKey RotAssistKey);

	void GetThrusterFXAndSound(UNiagaraSystem* TempNS_Thruster, USoundBase* TempSB_Thruster);

	// Declaring functions to handle translational movement
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

	// Boost Function Declaration
	UFUNCTION()
	void Boost(float AxisValue);

	// Stopping translational movement
	UFUNCTION()
	void TranslationBrake(float AxisValue);

	// Stopping rotational movement
	UFUNCTION()
	void RotationBrake(float AxisValue);

	// Functions to set the Strafe Axis variables (called in the RazorBackPawn.cpp file)
	UFUNCTION()
	void SetStrafeFwdAxisName(FName StrafeFwdAxis)
	{
		StrafeFwdAxisName = StrafeFwdAxis;
	}

	// Strafe axes
	UFUNCTION()
	void SetXStrafeAxisName(FName XStrafeAxisName)
	{
		XStrfAxisName = XStrafeAxisName;
	}

	UFUNCTION()
	void SetYStrafeAxisName(FName XStrafeAxisName)
	{
		YStrfAxisName = XStrafeAxisName;
	}

	UFUNCTION()
	void SetZStrafeAxisName(FName XStrafeAxisName)
	{
		ZStrfAxisName = XStrafeAxisName;
	}

	UFUNCTION()
	void SetXRotationAxisName(FName XRotationAxisName)
	{
		XRttnAxisName = XRotationAxisName;
	}

	UFUNCTION()
	void SetYRotationAxisName(FName YRotationAxisName)
	{
		YRttnAxisName = YRotationAxisName;
	}

	UFUNCTION()
	void SetZRotationAxisName(FName ZRotationAxisName)
	{
		ZRttnAxisName = ZRotationAxisName;
	}

	// Rotation and translation braking axis names
	UFUNCTION()
	void SetTranslationBrakeName(FName TranslationBrake)
	{
		TrnsltBrk = TranslationBrake;
	}

	UFUNCTION()
	void SetRotationBrakeName(FName RotationBrake)
	{
		RotBrk = RotationBrake;
	}

	UFUNCTION()
	void SetRollLeftAxisName(FName RollLeftAxis)
	{
		RollLftAxisName = RollLeftAxis;
	}

	UFUNCTION()
	void SetRollRightAxisName(FName RollRightAxis)
	{
		RollRgtAxisName = RollRightAxis;
	}

	// Directional rotation AxisValue for keyboard arrow key rotation
	UFUNCTION()
	void SetRotLeftAxisName(FName RotLeftAxis)
	{
		RotLftAxisName = RotLeftAxis;
	}

	UFUNCTION()
	void SetRotRightAxisName(FName RoltRightAxis)
	{
		RotRgtAxisName = RoltRightAxis;
	}

	UFUNCTION()
	void SetRotUpAxisName(FName RotUpAxis)
	{
		RotUpAxisName = RotUpAxis;
	}

	UFUNCTION()
	void SetRotDownAxisName(FName RotDownAxis)
	{
		RotDwnAxisName = RotDownAxis;
	}

	UFUNCTION()
	float GetAxisValue(FName AxisName)
	{
		return Owner->GetInputAxisValue(AxisName);
	}

	// Setting rotation every tick on RazorbackPawn
	UFUNCTION()
	FVector GetShipRotRate(FVector ActorRotationRate)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("(SFCS) ActorRotationRate FVECTOR:%s"), *ActorRotationRate.ToString()));
		return ShipRotRate = ActorRotationRate;
	}

	// Rotation Assist toggle
	UFUNCTION()
	bool RotationAssistToggle();

private:

	// Get the total rotation rate (World rotation)
	UFUNCTION()
	float GetRotationRateSumInDegrees();

	// FQuat rotation generator
	UFUNCTION()
	FQuat RotatorToQuaternion(FRotator FRotation);

	// Comparing axis
	UFUNCTION()
	bool IsAxisGreater(FName FirstAxis, FName SecondAxis);
	UFUNCTION()
	bool IsAxisEqual(FName FirstAxis, FName SecondAxis);

	// Boost Consumption and recovery Calculation
	UFUNCTION()
	float ConsumeRecoverBoostFuel(float AxisValue);

	// Calculating the max speed based on input scale
	UFUNCTION()
	void InputToThrust(TArray <UPhysicsThrusterComponent*> FirstThrusterSet, TArray <UPhysicsThrusterComponent*> SecondThrusterSet, TArray <FVector> FirstThrusterSetPosition, TArray <FVector> SecondThrusterSetPosition, float FirstAxisAccel, float SecondAxisAccel, FName ActuatorAxis, TArray <FRotator> FirstTRotation, TArray <FRotator> SecondTRotation, bool isRotation = false);

	// Initializing the private thruster relative position vectors variables
	UFUNCTION()
	TArray <FVector> SetThrusterPositionVector(TArray <UPhysicsThrusterComponent*> ThrusterSet, bool read = false);
	TArray <FRotator> SetThrusterFRotation(TArray <UPhysicsThrusterComponent*> ThrusterSet, bool read = false);

	// Printing the thruster positions and rotations on screen
	UFUNCTION()
	void ShowThrustersPositionVector(TArray <UPhysicsThrusterComponent*> ThrusterSet);

	// Calculating thrust
	UFUNCTION()
	float CalculateThrust(float AxisAccel);

	// Calculating Velocity and acceleration
	UFUNCTION()
	float CalculateVelocity();

	UFUNCTION()
	void MaxVelocityByDamping();

	UFUNCTION()
	float CalculateAcceleration(float DeltaTime);

	// Getting translational speed as a vector
	UFUNCTION()
	FVector GetForwardTranslationSpeed(bool printSpeed = false);

	UFUNCTION()
	float GetRotationAxisValue();

	// Getting the VFX and playing it 
	void PlayThrusterVFX(FName ThrusterName);

	//HUD update function
	UFUNCTION()
	void UpdateHUD();

	// --------------------------------------------------------- Variables ---------------------------------------------------------  \\

	//Declaring all movement FNames and getting from parent
	FName StrafeFwdAxisName;
	FName StrafeBwdAxisName;
	FName StrafeLftAxisName;
	FName StrafeRgtAxisName;
	FName StrafeUpAxisName;
	FName StrafeDwnAxisName;
	FName RollLftAxisName;
	FName RollRgtAxisName;
	FName RotLftAxisName;
	FName RotRgtAxisName;
	FName RotUpAxisName;
	FName RotDwnAxisName;

	FName XStrfAxisName;
	FName YStrfAxisName;
	FName ZStrfAxisName;

	FName XRttnAxisName;
	FName YRttnAxisName;
	FName ZRttnAxisName;
	
	FName TrnsltBrk;
	FName RotBrk;

	FKey RotAssistKey = "";

	FVector ShipRotRate;
	FRotator RotationBeginPlay;
	FRotator RotationVelocity;

	// Handling Parameters
	float FwdAccel = 0.0f;
	float BwdAccel = 0.0f;
	float YAccel = 0.0f;
	float ZAccel = 0.0f;
	float RollAccel = 0.0f;
	float RotAccel = 0.0f;
	float AngularDamping = 0.0f;

	// Boost variables
	float BoostFuelTank = 0.0f;
	float BoostConsumptionRate = 0.0f;
	float BoostRecoveryRate = 0.0f;
	float TranslationBoostIncrement = 0.0f;
	float RotationBoostIncrement = 0.0f;
	float CurrentBoostFuel = 0.0f;
	float TranslationFinalBoost = 1.0f;
	float RotationBoostCalculated = 1.0f;
	float MinBoostPercentRequired = 0.0f;
	float MinBoostRequired = 0.0f;
	float PreviousBoostFuelAmt = 0.0f;
	float RARotRateBoostThresholdConsume = 0.0f;
	float LinearDamping = 0.0f;
	float LinearDampingSpeedThreshold = 0.0f;
	bool IsRegening = false;
	bool IsBoosting = false;
	bool IsRotationThruster = false;

	// Rotation assist mode bool variable
	bool RAEnabled = false;

	// Thruster Position params 
	TArray <FVector> MainThrustersRelativePosition;
	TArray <FVector> RetroThrustersRelativePosition;
	TArray <FVector> LeftThrustersRelativePosition;
	TArray <FVector> RightThrustersRelativePosition;
	TArray <FVector> TopThrustersRelativePosition;
	TArray <FVector> BottomThrustersRelativePosition;
	TArray <FVector> LeftRotThrustersRelativePosition;
	TArray <FVector> RighRotThrustersRelativePosition;
	TArray <FVector> BackRotThrustersRelativePosition;
	TArray <FVector> FrontRotThrustersRelativePosition;
	TArray <FVector> LeftRollThrustersRelativePosition;
	TArray <FVector> RightRollThrustersRelativePosition;

	// Thruster Rotation params
	TArray <FRotator> MainThrustersRelativeRotation;
	TArray <FRotator> RetroThrustersRelativeRotation;
	TArray <FRotator> LeftThrustersRelativeRotation;
	TArray <FRotator> RightThrustersRelativeRotation;
	TArray <FRotator> TopThrustersRelativeRotation;
	TArray <FRotator> BottomThrustersRelativeRotation;
	TArray <FRotator> LeftRotThrustersRelativeRotation;
	TArray <FRotator> RighRotThrustersRelativeRotation;
	TArray <FRotator> BackRotThrustersRelativeRotation;
	TArray <FRotator> FrontRotThrustersRelativeRotation;
	TArray <FRotator> LeftRollThrustersRelativeRotation;
	TArray <FRotator> RightRollThrustersRelativeRotation;
	// Mass
	float Mass = 0.0f;

	// Velocity and acceleration related variables
	float CurrentVelocity = 0.0f;
	float LastVelocity = 0.0f;
	float Acceleration = 0.0f;

	// DeltaTime variable
	float MyDeltaTime = 0.0f;

	// Getting owner
	AActor* Owner;

	// Creating a HUD variable
	AShipHUD* ShipHUD;
};