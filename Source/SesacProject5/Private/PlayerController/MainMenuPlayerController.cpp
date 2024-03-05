// copyright by LootingEmployment


#include "PlayerController/MainMenuPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameInstance/EFSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Menu/GameLoadingUI.h"
#include "UI/Menu/LobbyUI.h"
#include "UI/Menu/OpeningWidget.h"
#include "UI/Menu/TitleUI.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	opening = CreateWidget<UOpeningWidget>(GetWorld(), openingWidget);
	title = CreateWidget<UTitleUI>(GetWorld(), titleUI);
	lobby = CreateWidget<ULobbyUI>(GetWorld(), lobbyUI);
	gameLoading = CreateWidget<UGameLoadingUI>(GetWorld(), gameLoadingUI);

	opening->AddToViewport();
	title->AddToViewport();
	title->SetVisibility(ESlateVisibility::Hidden);
	lobby->AddToViewport();
	lobby->SetVisibility(ESlateVisibility::Hidden);
	gameLoading->AddToViewport();
	gameLoading->SetVisibility(ESlateVisibility::Hidden);

	SetShowMouseCursor(true);

	if (GetGameInstance<UEFSGameInstance>()->IsPlayerLoggedIn())
	{
		OnLoginSuccess();
	}
}

void AMainMenuPlayerController::OnLoginSuccess()
{
	ShowLobby();
	HiddenTitle();
	HiddenOpening();
}

void AMainMenuPlayerController::OnJoinSessionFail()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPlayerController::OnJoinSessionFail"));
	HiddenGameLoading();
	ShowLobby();
}

void AMainMenuPlayerController::HiddenOpening()
{
	opening->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuPlayerController::ShowTitle()
{
	title->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuPlayerController::HiddenTitle()
{
	title->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuPlayerController::ShowLobby()
{
	lobby->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuPlayerController::HiddenLobby()
{
	lobby->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuPlayerController::ShowGameLoading()
{
	gameLoading->SetVisibility(ESlateVisibility::Visible);

	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	// {
	// 	// GetWorld()->ServerTravel("/Game/YMH/Level/Test_YMH?listen", true);
	// 	UGameplayStatics::OpenLevel(GetWorld(), "Test_YMH");
	// }), 10.0f, false, 5.f);
}

void AMainMenuPlayerController::HiddenGameLoading()
{
	gameLoading->SetVisibility(ESlateVisibility::Hidden);
}