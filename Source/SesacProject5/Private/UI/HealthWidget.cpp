// copyright by LootingEmployment


#include "UI/HealthWidget.h"

#include "Component/HealthComponent.h"
#include "UI/HealthSlotWidget.h"

void UHealthWidget::InitWidget(APawn* Pawn)
{
	if (UHealthComponent* HealthComponent = Pawn->GetComponentByClass<UHealthComponent>())
	{
		HeadSlot->InitWidget(HealthComponent);
		ThoraxSlot->InitWidget(HealthComponent);
		StomachSlot->InitWidget(HealthComponent);
		LeftArmSlot->InitWidget(HealthComponent);
		RightArmSlot->InitWidget(HealthComponent);
		LeftLegSlot->InitWidget(HealthComponent);
		RightLegSlot->InitWidget(HealthComponent);
	}
}