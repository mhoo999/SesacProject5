// copyright by LootingEmployment


#include "UI/Menu/LobbyUI.h"

#include "Components/Button.h"
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

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		GetWorld()->ServerTravel("/Game/YMH/Level/Test_YMH?listen", true);
	}), 5.0f, false, 5.f);
}

void ULobbyUI::OpenWorkBenchTab()
{
	UE_LOG(LogTemp, Warning, TEXT("Open WorkBench!"));
}

void ULobbyUI::MovetoTitle()
{
	pc->ShowTitle();
	pc->HiddenLobby();
}