// Copyright Epic Games, Inc. All Rights Reserved.


#include "HeroesGameModeBase.h"
#include "Cpp_Cam.h"
#include "Cpp_PlayerController.h"

AHeroesGameModeBase::AHeroesGameModeBase()
{
	DefaultPawnClass = AHeroesGameModeBase::StaticClass();
	PlayerControllerClass = ACpp_PlayerController::StaticClass();
}
