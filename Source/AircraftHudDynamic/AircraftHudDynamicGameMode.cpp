// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AircraftHudDynamicGameMode.h"
#include "AircraftHudDynamicPawn.h"

AAircraftHudDynamicGameMode::AAircraftHudDynamicGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AAircraftHudDynamicPawn::StaticClass();
}
