// copyright by LootingEmployment


#include "UI/TabWidget.h"

#include "Components/WidgetSwitcher.h"
#include "PlayerController/InGamePlayerController.h"
#include "UI/GearWidget.h"
#include "UI/HealthWidget.h"

void UTabWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
}

void UTabWidget::InitWidget(APawn* Pawn)
{
	GearWidget->InitWidget(Pawn);
	HealthWidget->InitWidget(Pawn);
}

FReply UTabWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab || InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (AInGamePlayerController* InGamePlayerController = GetOwningPlayer<AInGamePlayerController>())
		{
			InGamePlayerController->ToggleTabWidget();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UTabWidget::SwitchToGearWidget()
{
	WidgetSwitcher->SetActiveWidget(GearWidget);
}

void UTabWidget::RefreshQuestWidget_Implementation()
{
	
}