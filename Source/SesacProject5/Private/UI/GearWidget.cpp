// copyright by LootingEmployment


#include "UI/GearWidget.h"

#include "Component/EquipmentComponent.h"
#include "UI/StorageWidget.h"

void UGearWidget::Init(APawn* Pawn)
{
	Pawn->GetComponentByClass<UEquipmentComponent>()->OnInventoryChanged.AddUObject(this, &UGearWidget::UpdateInventory);
	
}

void UGearWidget::UpdateInventory(TArray<FStorage>& StorageArray)
{
	if (StorageArray.Num() <= 0)
	{
		return;
	}
	
	StorageWidget->SetStorage(StorageArray[0]);
}