// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/TitleUI.h"

#include "Components/Button.h"
#include "PlayerController/MainMenuPlayerController.h"
#include "UI/Menu/LobbyUI.h"

void UTitleUI::NativeConstruct()
{
	Super::NativeConstruct();

	pc = Cast<AMainMenuPlayerController>(GetWorld()->GetFirstPlayerController());
}

void UTitleUI::MovetoLobby()
{
	pc->ShowLobby();
	pc->HiddenTitle();
}

void UTitleUI::ExitGame()
{
	// FGenericPlatformMisc::RequestExit(true);
}
