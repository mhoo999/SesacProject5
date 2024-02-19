// copyright by LootingEmployment


#include "UI/InGame/InGameWidget.h"

#include "UI/HealthWidget.h"
#include "UI/InGame/EscapeWidget.h"
#include "UI/InGame/InteractWidget.h"

void UInGameWidget::InitWidget(APawn* Pawn)
{
	InteractWidget->InitWidget(Pawn);
	HealthWidget->InitWidget(Pawn);
	EscapeWidget->InitWidget(Pawn);
}