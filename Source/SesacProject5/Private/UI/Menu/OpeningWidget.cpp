// copyright by LootingEmployment


#include "UI/Menu/OpeningWidget.h"

#include "PlayerController/MainMenuPlayerController.h"

void UOpeningWidget::PressAnyKey()
{
	pc = Cast<AMainMenuPlayerController>(GetWorld()->GetFirstPlayerController());

	pc->ShowTitle();
	pc->HiddenOpening();
}
