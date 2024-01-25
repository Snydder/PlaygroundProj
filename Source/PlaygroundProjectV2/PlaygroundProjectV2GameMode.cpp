// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlaygroundProjectV2GameMode.h"
#include "PlaygroundProjectV2Pawn.h"

APlaygroundProjectV2GameMode::APlaygroundProjectV2GameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = APlaygroundProjectV2Pawn::StaticClass();
}
