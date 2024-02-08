// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/EOSGameModeBase.h"

#include "GameSession/EOSGameSession.h"
#include "PlayerController/EOSPlayerController.h"

AEOSGameModeBase::AEOSGameModeBase()
{
	bUseSeamlessTravel = true;
	PlayerControllerClass = AEOSPlayerController::StaticClass();
	GameSessionClass = AEOSGameSession::StaticClass();
}
