// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCS.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "RazorbackPawn.h"
#include "Engine/EngineTypes.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "ShipHUD.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
USFCS::USFCS()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USFCS::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	ShipHUD = Cast<AShipHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Owner->InitializeComponents();
}

// Called every frame
void USFCS::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MyDeltaTime = DeltaTime;
	Acceleration = CalculateAcceleration(MyDeltaTime);
	CurrentVelocity = CalculateVelocity();

	// Updating HUD every frame
	USFCS::UpdateHUD();
}

// (BeginPlay) Getting the Array of thrusters per axis, configured in the details panel of the owning component.

TArray <UPhysicsThrusterComponent*> USFCS::ThrusterInitializer(TArray <FComponentReference> ThrusterRef, TArray <UPhysicsThrusterComponent*> Thruster)
{
	for (int32 i = 0; i != ThrusterRef.Num(); ++i)
	{
		Thruster.Emplace(Cast<UPhysicsThrusterComponent>(Owner->GetDefaultSubobjectByName(ThrusterRef[i].ComponentProperty)));
		Thruster[i]->Activate();
		Thruster[i]->ThrustStrength = 0.0f;
		Thruster[i]->SetMobility(EComponentMobility::Movable);
	}
	return Thruster;
}

void USFCS::GetXThrusters(TArray <FComponentReference> MainThrustersRef, TArray <FComponentReference> RetroThrustersRef)
{
	MainThrusters = ThrusterInitializer(MainThrustersRef, MainThrusters);
	RetroThrusters = ThrusterInitializer(RetroThrustersRef, RetroThrusters);

	MainThrustersRelativePosition.Append(SetThrusterPositionVector(MainThrusters));
	MainThrustersRelativeRotation.Append(SetThrusterFRotation(MainThrusters));

	RetroThrustersRelativePosition.Append(SetThrusterPositionVector(RetroThrusters));
	RetroThrustersRelativeRotation.Append(SetThrusterFRotation(RetroThrusters));
}

void USFCS::GetYThrusters(TArray <FComponentReference> LeftThrustersRef, TArray <FComponentReference> RightThrustersRef)
{
	LeftThrusters = ThrusterInitializer(LeftThrustersRef, LeftThrusters);
	RightThrusters = ThrusterInitializer(RightThrustersRef, RightThrusters);

	LeftThrustersRelativePosition.Append(SetThrusterPositionVector(LeftThrusters));
	LeftThrustersRelativeRotation.Append(SetThrusterFRotation(LeftThrusters));

	RightThrustersRelativePosition.Append(SetThrusterPositionVector(RightThrusters));
	RightThrustersRelativeRotation.Append(SetThrusterFRotation(RightThrusters));
}

void USFCS::GetZThrusters(TArray <FComponentReference> TopThrustersRef, TArray <FComponentReference> BottomThrustersRef)
{
	TopThrusters = ThrusterInitializer(TopThrustersRef, TopThrusters);
	BottomThrusters = ThrusterInitializer(BottomThrustersRef, BottomThrusters);

	TopThrustersRelativePosition.Append(SetThrusterPositionVector(TopThrusters));
	TopThrustersRelativeRotation.Append(SetThrusterFRotation(TopThrusters));

	BottomThrustersRelativePosition.Append(SetThrusterPositionVector(BottomThrusters));
	BottomThrustersRelativeRotation.Append(SetThrusterFRotation(BottomThrusters));
}

void USFCS::GetRotThrusters(TArray <FComponentReference> LeftRotThrusterRef, TArray <FComponentReference> RightRotThrusterRef, TArray <FComponentReference> BackRotThrusterRef, TArray <FComponentReference> FrontRotThrusterRef)
{
	LeftRotThrusters = ThrusterInitializer(LeftRotThrusterRef, LeftRotThrusters);
	RightRotThrusters = ThrusterInitializer(RightRotThrusterRef, RightRotThrusters);
	BackRotThrusters = ThrusterInitializer(BackRotThrusterRef, BackRotThrusters);
	FrontRotThrusters = ThrusterInitializer(FrontRotThrusterRef, FrontRotThrusters);

	LeftRotThrustersRelativePosition.Append(SetThrusterPositionVector(LeftRotThrusters));
	LeftRotThrustersRelativeRotation.Append(SetThrusterFRotation(LeftRotThrusters));

	RighRotThrustersRelativePosition.Append(SetThrusterPositionVector(RightRotThrusters));
	RighRotThrustersRelativeRotation.Append(SetThrusterFRotation(RightRotThrusters));

	BackRotThrustersRelativePosition.Append(SetThrusterPositionVector(BackRotThrusters));
	BackRotThrustersRelativeRotation.Append(SetThrusterFRotation(BackRotThrusters));

	FrontRotThrustersRelativePosition.Append(SetThrusterPositionVector(FrontRotThrusters));
	FrontRotThrustersRelativeRotation.Append(SetThrusterFRotation(BackRotThrusters));
}

void USFCS::GetRollThrusters(TArray <FComponentReference> LeftRollThrustersRef, TArray <FComponentReference> RightRollThrustersRef)
{
	LeftRollThrusters = ThrusterInitializer(LeftRollThrustersRef, LeftRollThrusters);
	RightRollThrusters = ThrusterInitializer(RightRollThrustersRef, RightRollThrusters);

	LeftRollThrustersRelativePosition.Append(SetThrusterPositionVector(LeftRollThrusters));
	LeftRollThrustersRelativeRotation.Append(SetThrusterFRotation(LeftRollThrusters));

	RightRollThrustersRelativePosition.Append(SetThrusterPositionVector(RightRollThrusters));
	RightRollThrustersRelativeRotation.Append(SetThrusterFRotation(RightRollThrusters));
}

void USFCS::GetThrusterFXAndSound(UNiagaraSystem* TempNS_Thruster, USoundBase* TempSB_Thruster)
{
	NS_Thruster = TempNS_Thruster;
	SB_Thruster = TempSB_Thruster;
}

// (BeginPlay) Getting handling characteristics from the owner

void USFCS::GetHandlingParams(
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
	float TempLinearDampingSpeedThreshold
)
{
	FwdAccel = TempFwdAccel;
	BwdAccel = TempBwdAccel;
	YAccel = TempYAccel;
	ZAccel = TempZAccel;
	RollAccel = TempRollAccel;
	RotAccel = TempRotAccel;
	AngularDamping = TempAngularDamping;
	TranslationBoostIncrement = TempTranslationBoostIncrement;
	RotationBoostIncrement = TempRotationBoostIncrement;
	BoostFuelTank = TempBoostFuelTank;
	CurrentBoostFuel = BoostFuelTank;
	BoostConsumptionRate = TempBoostConsumptionRate;
	BoostRecoveryRate = TempBoostRecoveryRate;
	MinBoostPercentRequired = TempMinBoostPercentRequired;
	RARotRateBoostThresholdConsume = TempRARotRateBoostThresholdConsume;
	Mass = TempMass;
	LinearDamping= TempLinearDamping;
	LinearDampingSpeedThreshold = TempLinearDampingSpeedThreshold;

	// Converting the variable MinBoostRequired to a percentage of the total fuel tank amount. 
	MinBoostRequired = BoostFuelTank * (MinBoostPercentRequired / 100);

	// Initializing the variable PreviousBoostFuelAmt with the CurrentBoostFuel (Max of the fuel tank) at the beggining.
	PreviousBoostFuelAmt = CurrentBoostFuel;

	UE_LOG(LogTemp, Warning, TEXT("(SFCS) FwdAccel: %f"), FwdAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) YAccel: %f"), YAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) ZAccel: %f"), ZAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) RollAccel: %f"), RollAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) RotAccel: %f"), RotAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) TranslationBoostIncrement: %f"), TranslationBoostIncrement);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) TranslationBoostIncrement: %f"), RotationBoostIncrement);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) Mass: %f"), Mass);
}

bool USFCS::IsAxisGreater(FName FirstAxis, FName SecondAxis)
{
	return Owner->InputComponent->GetAxisValue(FirstAxis) > Owner->InputComponent->GetAxisValue(SecondAxis) ? true : false;
}

bool USFCS::IsAxisEqual(FName FirstAxis, FName SecondAxis)
{
	return Owner->InputComponent->GetAxisValue(FirstAxis) == Owner->InputComponent->GetAxisValue(SecondAxis) ? true : false;
}

// Translating Acceleration values into thrust values
float USFCS::CalculateThrust(float AxisAccel)
{
	float ThrustValue = 0.0f;

	// Force = mass * acceleration (in m/s)

	ThrustValue = Mass * (AxisAccel * 100);
	
	/*
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) ThrustValue: %f"), ThrustValue);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) AxisAccel: %f"), AxisAccel);
	UE_LOG(LogTemp, Warning, TEXT("(SFCS) Mass: %f"), Mass);
	*/

	return ThrustValue;
}


// Calculating Velocity
float USFCS::CalculateVelocity()
{
	CurrentVelocity = sqrt(pow(CubeOwnerStaticMesh->GetPhysicsLinearVelocity().X, 2) + pow(CubeOwnerStaticMesh->GetPhysicsLinearVelocity().Y, 2) + pow(CubeOwnerStaticMesh->GetPhysicsLinearVelocity().Z, 2)) / 100;
	
	// Calling the function to set a max velocity.
	MaxVelocityByDamping();

	return CurrentVelocity;
}

// Setting a max velocity by damping 
void USFCS::MaxVelocityByDamping()
{
	// If the Current velocity is bigger than the velocity threshold set, apply damping. If not, reset it to zero.
	(CurrentVelocity >= LinearDampingSpeedThreshold) ? CubeOwnerStaticMesh->SetLinearDamping(LinearDamping) : CubeOwnerStaticMesh->SetLinearDamping(0.0f);
}

// Calculating Acceleration
float USFCS::CalculateAcceleration(float DeltaTime)
{
	float tempAcceleration = 0.0f;
	float tempVelocity = CalculateVelocity();

	tempAcceleration = (tempVelocity - LastVelocity) / DeltaTime;
	LastVelocity = CalculateVelocity();

	return tempAcceleration;
}

// -----------------------------------------------//

// Handling Translational movement on each axis.

// Translate Forward and Backward
void USFCS::XThrust(float AxisValue)
{
	InputToThrust(MainThrusters, RetroThrusters, MainThrustersRelativePosition, RetroThrustersRelativePosition, FwdAccel, BwdAccel, XStrfAxisName, MainThrustersRelativeRotation, RetroThrustersRelativeRotation, false);
}

// Translate Left and Right
void USFCS::YThrust(float AxisValue)
{
	InputToThrust(LeftThrusters, RightThrusters, LeftThrustersRelativePosition, RightThrustersRelativePosition, YAccel, YAccel, YStrfAxisName, LeftThrustersRelativeRotation, RightThrustersRelativeRotation, false);
}

// Translate Up and Down
void USFCS::ZThrust(float AxisValue)
{
	InputToThrust(TopThrusters, BottomThrusters, TopThrustersRelativePosition, BottomThrustersRelativePosition, ZAccel, ZAccel, ZStrfAxisName, TopThrustersRelativeRotation, BottomThrustersRelativeRotation, false);
}

// -----------------------------------------------//

// Handling Rotational Movement

// Rotate Up and Down
void USFCS::YRotation(float AxisValue)
{
	InputToThrust(FrontRotThrusters, BackRotThrusters, FrontRotThrustersRelativePosition, BackRotThrustersRelativePosition, RotAccel, RotAccel, YRttnAxisName, FrontRotThrustersRelativeRotation, BackRotThrustersRelativeRotation, true);
}

// Rotate Left and right
void USFCS::ZRotation(float AxisValue)
{
	InputToThrust(RightRotThrusters, LeftRotThrusters, RighRotThrustersRelativePosition, LeftRotThrustersRelativePosition, RotAccel, RotAccel, ZRttnAxisName, RighRotThrustersRelativeRotation, LeftRotThrustersRelativeRotation, true);
}

// -----------------------------------------------//

// Roll left and right
void USFCS::XRotation(float AxisValue)
{
	InputToThrust(RightRollThrusters, LeftRollThrusters, RightRollThrustersRelativePosition, LeftRollThrustersRelativePosition, RollAccel, RollAccel, XRttnAxisName, RightRollThrustersRelativeRotation, LeftRollThrustersRelativeRotation, true);
}

// -----------------------------------------------//

// Setting thruster relative position
TArray <FVector> USFCS::SetThrusterPositionVector(TArray <UPhysicsThrusterComponent*> ThrusterSet, bool read)
{
	TArray <FVector> ThrusterRelLocation;

	for (int32 i = 0; i != ThrusterSet.Num(); ++i)
	{
		ThrusterRelLocation.Emplace(ThrusterSet[i]->GetRelativeLocation());
		if (read)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("ThrusterRelLocation X: %s"), *ThrusterRelLocation[i].ToString()));
			
	}
	return ThrusterRelLocation;
}

TArray <FRotator> USFCS::SetThrusterFRotation(TArray <UPhysicsThrusterComponent*> ThrusterSet, bool read)
{
	TArray <FRotator> ThrusterRelRotation;

	for (int32 i = 0; i != ThrusterSet.Num(); ++i)
	{
		ThrusterRelRotation.Emplace(ThrusterSet[i]->GetRelativeRotation());
			if (read)
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("ThrusterRelRotation X: %s"), *ThrusterRelRotation[i].ToString()));
	}
	return ThrusterRelRotation;
}

// Setting the thruster to the correct relative position and printing the thruster positions on screen if desired
void USFCS::ShowThrustersPositionVector(TArray <UPhysicsThrusterComponent*> ThrusterSet)
{
	for (int32 i = 0; i != ThrusterSet.Num(); ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Thruster'%s' relative location: %s"), *ThrusterSet[i]->GetName(), *ThrusterSet[i]->GetRelativeLocation().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Thruster'%s' relative rotation: %s"), *ThrusterSet[i]->GetName(), *ThrusterSet[i]->GetRelativeRotation().ToString()));
	}
}


// Transforming input into thrust
void USFCS::InputToThrust(TArray <UPhysicsThrusterComponent*> FirstThrusterSet, TArray <UPhysicsThrusterComponent*> SecondThrusterSet, TArray <FVector> FirstThrusterSetPosition, TArray <FVector> SecondThrusterSetPosition, float FirstAxisAccel, float SecondAxisAccel, FName ActuatorAxis, TArray <FRotator> FirstTRotation, TArray <FRotator> SecondTRotation, bool isRotation)
{
	float AxisValue = GetAxisValue(ActuatorAxis);
		if (AxisValue >= 0.0f)
		{
			if (AxisValue == 0.0f)
			{
				for (int32 i = 0; i != FirstThrusterSet.Num(); ++i)
				{
					FirstThrusterSet[i]->SetRelativeRotation(RotatorToQuaternion(FirstTRotation[i]), false, 0, ETeleportType::None);
					FirstThrusterSet[i]->SetRelativeLocation(FirstThrusterSetPosition[i]);
					FirstThrusterSet[i]->ThrustStrength = 0.0f;
				}
			}
			else
			{
				for (int32 i = 0; i != FirstThrusterSet.Num(); ++i)
				{
					FirstThrusterSet[i]->SetRelativeRotation(RotatorToQuaternion(FirstTRotation[i]), false, 0, ETeleportType::None);
					FirstThrusterSet[i]->SetRelativeLocation(FirstThrusterSetPosition[i]);
					if (isRotation)
					{
						FirstThrusterSet[i]->ThrustStrength = FMath::Clamp(AxisValue, 0.0f, 1.0f) * (CalculateThrust(FirstAxisAccel) * RotationFinalBoost);
					}
					else
					{
						FirstThrusterSet[i]->ThrustStrength = FMath::Clamp(AxisValue, 0.0f, 1.0f) * (CalculateThrust(FirstAxisAccel) * TranslationFinalBoost);
					}

					PlayThrusterVFX(FName(FirstThrusterSet[i]->GetName()));
				}
			}
			
		}
		if (AxisValue <= 0.0f)
		{
			if (AxisValue == 0.0f)
			{
				for (int32 i = 0; i != SecondThrusterSet.Num(); ++i)
				{
					SecondThrusterSet[i]->SetRelativeRotation(SecondTRotation[i], false, 0, ETeleportType::None);
					FirstThrusterSet[i]->SetRelativeLocation(SecondThrusterSetPosition[i]);
					SecondThrusterSet[i]->ThrustStrength = 0.0f;
				}
			}
			else
			{
				for (int32 i = 0; i != SecondThrusterSet.Num(); ++i)
				{
					SecondThrusterSet[i]->SetRelativeRotation(SecondTRotation[i], false, 0, ETeleportType::None);
					SecondThrusterSet[i]->SetRelativeLocation(SecondThrusterSetPosition[i]);
					if (isRotation)
					{
						SecondThrusterSet[i]->ThrustStrength = FMath::Clamp(AxisValue, -1.0f, 0.0f) * (CalculateThrust(SecondAxisAccel) * RotationFinalBoost);
					}
					else
					{
						SecondThrusterSet[i]->ThrustStrength = FMath::Clamp(AxisValue, -1.0f, 0.0f) * (CalculateThrust(SecondAxisAccel) * TranslationFinalBoost);
					}

					SecondThrusterSet[i]->ThrustStrength *= -1;
					PlayThrusterVFX(FName(SecondThrusterSet[i]->GetName()));
				}
			}
		}
}

// -----------------------------------------------//

// Handling Boost functionality

void USFCS::Boost(float AxisValue)
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Fuel tank capacity: %f"), BoostFuelTank));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("(SFCS) CurrentFuel: %f"), CurrentBoostFuel));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("(SFCS) PreviousBoostFuelAmt: %f"), PreviousBoostFuelAmt));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("(SFCS) MinBoostRequired: %f"), MinBoostRequired));
	*/

	if (AxisValue != 0.0f && CurrentBoostFuel > 0.0f)
	{
		if (PreviousBoostFuelAmt > MinBoostRequired)
		{
			RotationFinalBoost = RotationBoostIncrement;
			TranslationFinalBoost = TranslationBoostIncrement;
			ConsumeRecoverBoostFuel(AxisValue);
		}
		else
		{
			ConsumeRecoverBoostFuel(0.0f);
		}
	}
	
	else if (AxisValue == 0.0f && CurrentBoostFuel < BoostFuelTank)
	{
		RotationFinalBoost = 1.0f;
		TranslationFinalBoost = 1.0f;
		if (RAEnabled && (GetRotationAxisValue() > 0.0f || GetRotationAxisValue() < 0.0f) || !RAEnabled && GetAxisValue(RotBrk))
		{
			// Do nothing
		}
		else
		{
			ConsumeRecoverBoostFuel(AxisValue);
			PreviousBoostFuelAmt = CurrentBoostFuel;
		}
	}


	// Setting bool variable for the UI
	(AxisValue != 0.0f  && CurrentBoostFuel > MinBoostRequired) ? IsBoosting = true : IsBoosting = false;
}

// -----------------------------------------------//

void USFCS::TranslationBrake(float AxisValue)
{
	bool isBraking = false; 
	float XVelocity = GetForwardTranslationSpeed().X;
	float YVelocity = GetForwardTranslationSpeed().Y;
	float ZVelocity = GetForwardTranslationSpeed().Z;

	if (AxisValue != 0.0f)
	{
		isBraking = true;

		// Forward and backward 
		if (XVelocity > 0.0f)
			InputToThrust(RetroThrusters, RetroThrusters, RetroThrustersRelativePosition, RetroThrustersRelativePosition, BwdAccel, BwdAccel, TrnsltBrk, RetroThrustersRelativeRotation, RetroThrustersRelativeRotation);
		else if (XVelocity < 0.0f)
			InputToThrust(MainThrusters, MainThrusters, MainThrustersRelativePosition, MainThrustersRelativePosition, FwdAccel, FwdAccel, TrnsltBrk, MainThrustersRelativeRotation, MainThrustersRelativeRotation);
		
		// Left and right
		if (YVelocity > 0.0f)
			InputToThrust(RightThrusters, RightThrusters, RightThrustersRelativePosition, RightThrustersRelativePosition, YAccel, YAccel, TrnsltBrk, RightThrustersRelativeRotation, RightThrustersRelativeRotation);
		else if (YVelocity < 0.0f)
			InputToThrust(LeftThrusters, LeftThrusters, LeftThrustersRelativePosition, LeftThrustersRelativePosition, YAccel, YAccel, TrnsltBrk, LeftThrustersRelativeRotation, LeftThrustersRelativeRotation);

		// Up and down 
		if (ZVelocity > 0.0f)
			InputToThrust(TopThrusters, TopThrusters, TopThrustersRelativePosition, TopThrustersRelativePosition, ZAccel, ZAccel, TrnsltBrk, TopThrustersRelativeRotation, TopThrustersRelativeRotation);
		else if (ZVelocity < 0.0f)
			InputToThrust(BottomThrusters, BottomThrusters, BottomThrustersRelativePosition, BottomThrustersRelativePosition, ZAccel, ZAccel, TrnsltBrk, BottomThrustersRelativeRotation, BottomThrustersRelativeRotation);
		}
}

// -----------------------------------------------//

void USFCS::RotationBrake(float AxisValue)
{
	if (!RAEnabled)
	{
		if (AxisValue != 0.0f)
		{
			// Comparing if the rotation rate is bigger than the threshold specified and the input is zero.
			if ((GetRotationRateSumInDegrees() > RARotRateBoostThresholdConsume || GetRotationRateSumInDegrees() < FMath::Abs(RARotRateBoostThresholdConsume) * -1) && GetRotationAxisValue() == 0.0f)
			{
				ConsumeRecoverBoostFuel(AxisValue);
			}
			else
			{
				ConsumeRecoverBoostFuel(0.0f);
			}
			// Setting damping while also taking into account if the user is boosting (higher dampening)
			CubeOwnerStaticMesh->SetAngularDamping(AngularDamping * RotationFinalBoost);
		}
		else
		{
			CubeOwnerStaticMesh->SetAngularDamping(0.0f);
		}
	}
	else
	{
		// Setting damping while also taking into account if the user is boosting (higher dampening)
		(IsBoosting) ? CubeOwnerStaticMesh->SetAngularDamping(AngularDamping * RotationFinalBoost): CubeOwnerStaticMesh->SetAngularDamping(AngularDamping);

		// Comparing if the rotation rate is bigger than the threshold specified and the input is zero.
		if ((GetRotationRateSumInDegrees() > RARotRateBoostThresholdConsume || GetRotationRateSumInDegrees() < FMath::Abs(RARotRateBoostThresholdConsume) * -1) && GetRotationAxisValue() == 0.0f)
		{
			ConsumeRecoverBoostFuel(1.0f);
		}
	}
}

// Getting Translational relative speed in m/s
FVector USFCS::GetForwardTranslationSpeed(bool printSpeed)
{
	FVector V = CubeOwnerStaticMesh->GetPhysicsLinearVelocity();
	FRotator R = CubeOwnerStaticMesh->GetComponentRotation();
	V = R.UnrotateVector(V) / 100;

	float XVelocity = V.X;
	float YVelocity = V.Y;
	float ZVelocity = V.Z;
	if (printSpeed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Speed X: %.2f m/s"), XVelocity));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Speed Y: %.2f m/s"), YVelocity));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Speed Z: %.2f m/s"), ZVelocity));
	}
	return V;
}

// Calculating boost consumption rate
float USFCS::ConsumeRecoverBoostFuel(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		if (CurrentBoostFuel >= 0.0f)
		{ 
			CurrentBoostFuel -= BoostConsumptionRate;
 			CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.0f, BoostFuelTank);
		}
		if (CurrentBoostFuel > MinBoostRequired)
		{
			PreviousBoostFuelAmt = CurrentBoostFuel;
		}
		IsRegening = false;
	}
	else
	{
		if (CurrentBoostFuel < BoostFuelTank)
		{
			CurrentBoostFuel += BoostRecoveryRate;
			CurrentBoostFuel = FMath::Clamp(CurrentBoostFuel, 0.0f, BoostFuelTank);
			IsRegening = true;
			if (CurrentBoostFuel ==  BoostFuelTank)
				IsRegening = false;
		}
	}

	return CurrentBoostFuel;
}

// Generating a FQuat rotation
FQuat USFCS::RotatorToQuaternion(FRotator FRotation)
{
	FQuat QuatRotation = FQuat(FRotation);
	return QuatRotation;
}

// Sum all axis values
float USFCS::GetRotationAxisValue()
{
	float TotalAxisValue = GetAxisValue(ZRttnAxisName) + GetAxisValue(YRttnAxisName) + GetAxisValue(XRttnAxisName);
	return TotalAxisValue;
}

// Rotation assist mode 
bool USFCS::RotationAssistToggle()
{
	return RAEnabled = (!RAEnabled) ? true : false;
}

float USFCS::GetRotationRateSumInDegrees()
{
	return CubeOwnerStaticMesh->GetPhysicsAngularVelocityInRadians().X + CubeOwnerStaticMesh->GetPhysicsAngularVelocityInRadians().Y + CubeOwnerStaticMesh->GetPhysicsAngularVelocityInRadians().Z;
}

void USFCS::PlayThrusterVFX(FName ThrusterName)
{
	FVector tempSocketLocation = VisualOwnerStaticMesh->GetSocketLocation(ThrusterName);
	FRotator tempSocketRotation = VisualOwnerStaticMesh->GetSocketRotation(ThrusterName);

	if (NS_Thruster)
		if (VisualOwnerStaticMesh->GetSocketByName(ThrusterName))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Thruster, tempSocketLocation, tempSocketRotation);
		}
}

// Setting keybinding button label variable
void USFCS::SetBindingKeys(FKey tempRotAssistKey)
{
	RotAssistKey = tempRotAssistKey;
}

void USFCS::UpdateHUD()
{

	// Checking if the HUD is initialized, if so proceed with the following instructions.
	if (ShipHUD)
	{
		ShipHUD->UpdateCurrentFuel(BoostFuelTank, CurrentBoostFuel, MinBoostPercentRequired);
		ShipHUD->UpdateCurrentHandlingStats(CurrentVelocity, Acceleration, IsRegening, RAEnabled, RotAssistKey);
	}
}
