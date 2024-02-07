// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/InGamePlayerController.h"

#include "GameInstance/EFSGameInstance.h"

void AInGamePlayerController::OnNetCleanup(UNetConnection* Connection)
{
	UEFSGameInstance* GameInstance = GetGameInstance<UEFSGameInstance>();
	if (GameInstance)
	{
		GameInstance->DestroySession();
	}

	Super::OnNetCleanup(Connection);
}
