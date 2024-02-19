// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ---------------- title ---------------------
	btn_gameStart->OnClicked.AddDynamic(this, &UMyUserWidget::MovetoLobby);
	btn_exit->OnClicked.AddDynamic(this, &UMyUserWidget::ExitGame);

	// ---------------- Lobby ---------------------
	btn_Inventory->OnClicked.AddDynamic(this, &UMyUserWidget::OpenInventory);
	btn_city->OnClicked.AddDynamic(this, &UMyUserWidget::OpenCityTab);
	btn_movetoField->OnClicked.AddDynamic(this, &UMyUserWidget::MovetoField);
	btn_workBench->OnClicked.AddDynamic(this, &UMyUserWidget::OpenWorkBenchTab);
	btn_back->OnClicked.AddDynamic(this, &UMyUserWidget::MovetoTitle);

	
}

void UMyUserWidget::MovetoLobby()
{
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void UMyUserWidget::ExitGame()
{
	FGenericPlatformMisc::RequestExit(true);
}

void UMyUserWidget::OpenInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Inventory!"));
}

void UMyUserWidget::OpenCityTab()
{
	UE_LOG(LogTemp, Warning, TEXT("Open City!"));
}

void UMyUserWidget::MovetoField()
{
	widgetSwitcher->SetActiveWidgetIndex(2);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		GetWorld()->ServerTravel("/Game/YMH/Level/Test_YMH?listen", true);
	}), 5.0f, false, 5.f);
}

void UMyUserWidget::OpenWorkBenchTab()
{
	UE_LOG(LogTemp, Warning, TEXT("Open WorkBench!"));
}

void UMyUserWidget::MovetoTitle()
{
	widgetSwitcher->SetActiveWidgetIndex(0);
}
