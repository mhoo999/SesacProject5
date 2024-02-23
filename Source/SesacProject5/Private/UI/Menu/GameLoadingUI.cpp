// copyright by LootingEmployment


#include "UI/Menu/GameLoadingUI.h"

#include "PlayerController/MainMenuPlayerController.h"

void UGameLoadingUI::NativeConstruct()
{
	Super::NativeConstruct();

	pc = Cast<AMainMenuPlayerController>(GetWorld()->GetFirstPlayerController());

}
