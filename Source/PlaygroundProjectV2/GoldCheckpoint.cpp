// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldCheckpoint.h"
#include "Components/BoxComponent.h"
//#include "../../../../Program Files (x86)/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
//#include "NiagaraSystem.h"
#include "RazorbackPawn.h"
#include "Engine/Engine.h"

// Sets default values
AGoldCheckpoint::AGoldCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoldCheckpointVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GoldCheckpoint"));
	RootComponent = GoldCheckpointVFX;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetCollisionProfileName("Trigger");
	GoldCheckpointVFX->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGoldCheckpoint::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGoldCheckpoint::OnOverlapEnd);

	CollisionBox->SetupAttachment(GoldCheckpointVFX);

}

// Called when the game starts or when spawned
void AGoldCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	// Initializing the ShipHUD variable
	ShipHUD = Cast<AShipHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	TAGM = Cast<ATimeAttackGamemode>(GetWorld()->GetAuthGameMode());
	TAGS = Cast<ATimeAttackGameState>(GetWorld()->GetGameState());

	// Adding to the total
	TAGM->SetTotalCheckpointCount(1);


}

// Called every frame
void AGoldCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TAGS->GetRemainingTime();

	if (!TAGS->GetWorldTimerManager().IsTimerActive(TAGS->TimerHandle))
	{
		if (GoldCheckpointVFX)
			GoldCheckpointVFX->DestroyComponent();
		if (CollisionBox)
		CollisionBox->DestroyComponent();
		//TAGS->GetWorldTimerManager().ClearTimer(TAGS->TimerHandle);
	}
}

void AGoldCheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TAGM->SetCurrentCheckpointCount();

	if (TAGM->GetCurrentCheckpointCount() == TAGM->GetTotalCheckpointCount())
	{
		TAGS->TimeTracker = TAGS->GetRemainingTime();
	}
	else
	TAGS->IncrementResetTimer();
}

void AGoldCheckpoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GoldCheckpointVFX->DestroyComponent();
	CollisionBox->DestroyComponent();
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Exited")));
}

