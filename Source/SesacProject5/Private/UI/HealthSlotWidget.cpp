// copyright by LootingEmployment


#include "UI/HealthSlotWidget.h"

#include "Component/HealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TB_SlotName->SetText(SlotName);
}

void UHealthSlotWidget::InitWidget(UHealthComponent* HealthComponent)
{
	HealthComponent->GetHealth(BodyParts).OnHealthChanged.BindUObject(this, &UHealthSlotWidget::UpdateWidget);
}

void UHealthSlotWidget::UpdateWidget(float Health, float MaxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("UHealthSlotWidget::UpdateWidget) %s"), *UEnum::GetValueAsString(BodyParts));
	TB_Health->SetText(FText::AsNumber((int)Health));
	TB_MaxHealth->SetText(FText::AsNumber((int)MaxHealth));
	PB_Health->SetPercent(Health / MaxHealth);
}