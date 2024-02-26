// copyright by LootingEmployment


#include "UI/InGame/InGameWidget.h"

#include "Components/WidgetSwitcher.h"
#include "UI/HealthWidget.h"
#include "UI/TabWidget.h"
#include "UI/InGame/EscapeWidget.h"
#include "UI/InGame/InteractWidget.h"

void UInGameWidget::InitWidget(APawn* Pawn)
{
	InteractWidget->InitWidget(Pawn);
	HealthWidget->InitWidget(Pawn);
	EscapeWidget->InitWidget(Pawn);
	TabWidget->InitWidget(Pawn);
}

void UInGameWidget::ToggleTabWidget()
{
	if (WidgetSwitcher->GetActiveWidget() == InteractWidget)
	{
		// InteractWidget is Active

		
		// Switch to Tab Widget	
		APlayerController* PlayerController = GetOwningPlayer();
		PlayerController->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TabWidget->GetCachedWidget());
		PlayerController->SetInputMode(InputMode);
		WidgetSwitcher->SetActiveWidget(TabWidget);
		InteractWidget->StopInteract();
	}
	else if (WidgetSwitcher->GetActiveWidget() == TabWidget)
	{
		// TabWidget is Active

		// Switch to InteractWidget	
		APlayerController* PlayerController = GetOwningPlayer();
		PlayerController->SetShowMouseCursor(false);
        PlayerController->SetInputMode(FInputModeGameOnly());
		WidgetSwitcher->SetActiveWidget(InteractWidget);
		InteractWidget->StartInteract();
	}
}