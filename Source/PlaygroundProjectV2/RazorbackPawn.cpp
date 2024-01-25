// Fill out your copyright notice in the Description page of Project Settings.

#include "RazorbackPawn.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "SFCS.h"
#include "Components/ActorComponent.h"

// Sets default values
ARazorbackPawn::ARazorbackPawn()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Self = GetOwner();

	// Creating subobjects
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
	VisualMesh = CreateDefaultSubobject <UStaticMeshComponent>("VisualMesh");
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	//Setting the root component
	SetRootComponent(CubeMesh);

	VisualMesh->SetupAttachment(CubeMesh);

	CameraSpringArm->SetupAttachment(CubeMesh);
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);


	//Creating Flight Controller 
	SFCS = CreateDefaultSubobject<USFCS>("SFCS");
}

// Called when the game starts or when spawned
void ARazorbackPawn::BeginPlay()
{
	Super::BeginPlay();

	// Adding flight component to the ship on BeginPlay()
	AddOwnedComponent(SFCS);

	// Intializing the gamemode variable
	TAGM = Cast<ATimeAttackGamemode>(GetWorld()->GetAuthGameMode());

	if (!CubeMesh->HasBeenInitialized())
		CubeMesh->InitializeComponent();

	if (!VisualMesh->HasBeenInitialized())
		VisualMesh->InitializeComponent();
	
	if (!Camera->HasBeenInitialized())
		Camera->InitializeComponent();
	
	if (!CameraSpringArm->HasBeenInitialized())
		CameraSpringArm->InitializeComponent();

	SFCS->SetCubeOwnerStaticMesh(CubeMesh);
	SFCS->SetVisualOwnerStaticMesh(VisualMesh);

	Self = SFCS->GetOwner();

	if (CubeMesh->HasBeenInitialized())
	{
		if (CubeMesh->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Is ShipMesh valid: true"));

			FBodyInstance* CubeMeshBodyInst = CubeMesh->GetBodyInstance();

			// Setting all attributes on game start
			CubeMesh->SetSimulatePhysics(true);
			CubeMesh->SetEnableGravity(true);
			CubeMeshBodyInst->MassScale = MassScale;
			CubeMeshBodyInst->UpdateMassProperties();

			VisualMesh->SetSimulatePhysics(false);
			VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			CameraSpringArm->SetTickGroup(ETickingGroup::TG_PostPhysics);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Is ShipMesh valid: false"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Is ShipMesh intialized: true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Is ShipMesh intialized: false"));
	}

	// Setting Spring Arm Properties
	CameraSpringArm->Activate();
	CameraSpringArm->TargetArmLength = TargetArmLength;
	CameraSpringArm->bEnableCameraLag = EnableCameraLag;
	CameraSpringArm->bEnableCameraRotationLag = EnableCameraRotationLag;
	CameraSpringArm->CameraLagSpeed = CameraLagSpeed;
	CameraSpringArm->CameraRotationLagSpeed = CameraRotationLagSpeed;

	UE_LOG(LogTemp, Warning, TEXT("(CubeMesh) Is simulating physics: %d"), CubeMesh->IsSimulatingPhysics());
	UE_LOG(LogTemp, Warning, TEXT("(CubeMesh) Is gravity enabled: %d"), CubeMesh->IsGravityEnabled());
	UE_LOG(LogTemp, Warning, TEXT("(VisualMesh) Is simulating physics: %d"), VisualMesh->IsSimulatingPhysics());
	UE_LOG(LogTemp, Warning, TEXT("(VisualMesh) Is gravity enabled: %d"), VisualMesh->IsGravityEnabled());


	if (SFCS->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("SFCS is valid."));
	}

	// Sending handling parameters to the SFCS
	SFCS->GetHandlingParams(FwdAccel, BwdAccel, YAccel, ZAccel, RollAccel, RotAccel, AngularDamping, TranslationBoostIncrement, RotationBoostIncrement, BoostFuelTank, BoostConsumptionRate, BoostRecoveryRate, MinBoostPercentRequired, RotationAssistRotRateBoostThresholdConsume, CubeMesh->GetMass(), LinearDamping, LinearDampingSpeedThreshold);

	// Sending the VFX and sounds to the SFCS
	SFCS->GetThrusterFXAndSound(NS_Thruster, SB_Thruster);

	// Setting Fnames with InputAxis name; Will be used to get if axis is being used
	SFCS->SetRollLeftAxisName(NameRollLeft);
	SFCS->SetRollRightAxisName(NameRollRight);
	SFCS->SetRotLeftAxisName(NameRotLeft);
	SFCS->SetRotRightAxisName(NameRotRight);
	SFCS->SetRotUpAxisName(NameRotUp);
	SFCS->SetRotDownAxisName(NameRotDown);

	SFCS->SetXStrafeAxisName(NameXStrafe);
	SFCS->SetYStrafeAxisName(NameYStrafe);
	SFCS->SetZStrafeAxisName(NameZStrafe);

	SFCS->SetXRotationAxisName(NameXRotation);
	SFCS->SetYRotationAxisName(NameYRotation);
	SFCS->SetZRotationAxisName(NameZRotation);

	SFCS->SetTranslationBrakeName(TranslatBrk);
	SFCS->SetRotationBrakeName(RotBrk);

	// Check for empty thruster arrays, then get all thrusters on SFCS system (if every array is filled)
	if (ARazorbackPawn::ThrusterArraySweep())
	{
		SFCS->GetXThrusters(MainThrustersArray, RetroThrustersArray);
		SFCS->GetYThrusters(LeftThrustersArray, RightThrustersArray);
		SFCS->GetZThrusters(TopThrustersArray, BottomThrustersArray);
		SFCS->GetRotThrusters(LeftRotThrustersArray, RightRotThrustersArray, BackRotThrustersArray, FrontRotThrustersArray);
		SFCS->GetRollThrusters(LeftRollThrustersArray, RightRollThrustersArray);
	}

	// Get the instance of the InputSettings
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	// ActionMappings with all the input binding information is stored here
	TArray<FInputActionKeyMapping> ActionKeys;

	// Gets the mapping of the ActionMapping by the name and stores into the array
	InputSettings->GetActionMappingByName(NameRotationAssistMode, ActionKeys);

	// Sends the information into the SFCS function, which forwards it to the UI.
	SFCS->SetBindingKeys(ActionKeys[0].Key);

}

// Called every frame
void ARazorbackPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARazorbackPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Respond when our "Boost" key is pressed or released.
	InputComponent->BindAxis("Boost", this, &ARazorbackPawn::Boost);

	// Respond every frame to the values of our movement axes
	InputComponent->BindAxis(NameXStrafe, this, &ARazorbackPawn::XThrust);
	InputComponent->BindAxis(NameYStrafe, this, &ARazorbackPawn::YThrust);
	InputComponent->BindAxis(NameZStrafe, this, &ARazorbackPawn::ZThrust);

	InputComponent->BindAxis(NameXRotation, this, &ARazorbackPawn::XRotation);
	InputComponent->BindAxis(NameYRotation, this, &ARazorbackPawn::YRotation);
	InputComponent->BindAxis(NameZRotation, this, &ARazorbackPawn::ZRotation);

	InputComponent->BindAxis(TranslatBrk, this, &ARazorbackPawn::TranslationBrake);
	InputComponent->BindAxis(RotBrk, this, &ARazorbackPawn::RotationBrake);
	InputComponent->BindAction(NameRotationAssistMode, IE_Pressed, this, &ARazorbackPawn::RotationAssistToggle);
	InputComponent->BindAction(NameRestartGame, IE_Pressed, this, &ARazorbackPawn::RestartGamemode);
}

bool ARazorbackPawn::ThrusterArraySweep()
{
	bool AllArraysfilled = true;

	if (!TopThrustersArray.IsValidIndex(0))
	{
		TopThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!BottomThrustersArray.IsValidIndex(0))
	{
		BottomThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!LeftThrustersArray.IsValidIndex(0))
	{
		LeftThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!RightThrustersArray.IsValidIndex(0))
	{
		RightThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!MainThrustersArray.IsValidIndex(0))
	{
		MainThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!RetroThrustersArray.IsValidIndex(0))
	{
		RetroThrArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!LeftRollThrustersArray.IsValidIndex(0))
	{
		LeftRollThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!RightRollThrustersArray.IsValidIndex(0))
	{
		RightRollThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!LeftRotThrustersArray.IsValidIndex(0))
	{
		LeftRotThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!RightRotThrustersArray.IsValidIndex(0))
	{
		RightRotThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!BackRotThrustersArray.IsValidIndex(0))
	{
		BackRotThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	if (!FrontRotThrustersArray.IsValidIndex(0))
	{
		FrontRotThrustersArrayFilled = false;
		AllArraysfilled = false;
	}

	return AllArraysfilled;
}
void ARazorbackPawn::XThrust(float AxisValue)
{
	if (MainThrArrayFilled == true)
	{
		SFCS->XThrust(AxisValue);
	}
}

void ARazorbackPawn::YThrust(float AxisValue)
{
	if (MainThrArrayFilled == true)
	{
		SFCS->YThrust(AxisValue);
	}
}

void ARazorbackPawn::ZThrust(float AxisValue)
{
	if (MainThrArrayFilled == true)
	{
		SFCS->ZThrust(AxisValue);
	}
}

void ARazorbackPawn::XRotation(float AxisValue)
{
	if (LeftRollThrustersArrayFilled == true)
	{
		SFCS->XRotation(AxisValue);
	}
}

void ARazorbackPawn::ZRotation(float AxisValue)
{
	if (LeftRollThrustersArrayFilled == true)
	{
		SFCS->ZRotation(AxisValue);
	}
}

void ARazorbackPawn::YRotation(float AxisValue)
{
	if (LeftRollThrustersArrayFilled == true)
	{
		SFCS->YRotation(AxisValue);
	}
}

void ARazorbackPawn::Boost(float AxisValue)
{
	SFCS->Boost(AxisValue);
}

void ARazorbackPawn::TranslationBrake(float AxisValue)
{
	SFCS->TranslationBrake(AxisValue);
}

void ARazorbackPawn::RotationBrake(float AxisValue)
{
	SFCS->RotationBrake(AxisValue);
}

void ARazorbackPawn::RotationAssistToggle(FKey tempFKey)
{
	RotAssistKey = tempFKey;
	//UE_LOG(LogTemp, Warning, TEXT("(Razorbackpawn) keyname: %s"), *RotAssistKey.GetFName().ToString());
	SFCS->RotationAssistToggle();
}

void ARazorbackPawn::RestartGamemode()
{
	// Does not work
	TAGM->GetGameState<ATimeAttackGameState>()->Reset();
}