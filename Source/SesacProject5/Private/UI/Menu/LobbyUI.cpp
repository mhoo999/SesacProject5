// copyright by LootingEmployment


#include "UI/Menu/LobbyUI.h"

#include "PlayerController/MainMenuPlayerController.h"
#include "UI/Menu/TitleUI.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	pc = Cast<AMainMenuPlayerController>(GetWorld()->GetFirstPlayerController());
}

void ULobbyUI::OpenInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Inventory!"));
}

void ULobbyUI::OpenCityTab()
{
	UE_LOG(LogTemp, Warning, TEXT("Open City!"));
}

void ULobbyUI::MovetoField()
{
	pc->ShowGameLoading();
	pc->HiddenLobby();
}

void ULobbyUI::OpenWorkBenchTab()
{
}

void ULobbyUI::MovetoTitle()
{
	pc->ShowTitle();
	pc->HiddenLobby();
}