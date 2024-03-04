// copyright by LootingEmployment


#include "PlayerController/GameResultWidgetController.h"

#include "Blueprint/UserWidget.h"

void AGameResultWidgetController::BeginPlay()
{
	Super::BeginPlay();

	result = CreateWidget<UUserWidget>(this, resultWidget);
	result->AddToViewport();

	SetShowMouseCursor(true);

}

void AGameResultWidgetController::playSuccessMontage()
{
}

void AGameResultWidgetController::playFailMontage()
{
}
