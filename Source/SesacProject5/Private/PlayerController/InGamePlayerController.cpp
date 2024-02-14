// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/InGamePlayerController.h"
#include "UI/InGame/InGameWidget.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AInGamePlayerController::BeginPlay"));

	if (IsLocalController())
	{
		if (InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
			InGameWidget->AddToViewport();
			InGameWidget->InitWidget(GetPawn());
		}
	}
}