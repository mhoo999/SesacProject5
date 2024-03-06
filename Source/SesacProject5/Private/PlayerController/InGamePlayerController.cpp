// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/InGamePlayerController.h"
#include "UI/InGame/InGameWidget.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG(LogTemp, Warning, TEXT("AInGamePlayerController::BeginPlay"));

	if (IsLocalController())
	{
		PlayerCameraManager->ViewPitchMax = 60.f;
		PlayerCameraManager->ViewPitchMin = -60.f;
		if (InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
			InGameWidget->AddToViewport();
			InGameWidget->InitWidget(GetPawn());
		}
	}
}

void AInGamePlayerController::ToggleTabWidget()
{
	InGameWidget->ToggleTabWidget();
	BP_ToggleTabWidget();
}

void AInGamePlayerController::BP_ToggleTabWidget_Implementation()
{
}