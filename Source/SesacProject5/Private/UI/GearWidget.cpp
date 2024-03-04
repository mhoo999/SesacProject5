// copyright by LootingEmployment


#include "UI/GearWidget.h"

#include "Component/EquipmentComponent.h"
#include "UI/StorageWidget.h"

void UGearWidget::InitWidget(APawn* Pawn)
{
	UEquipmentComponent* EquipmentComponent = Pawn->GetComponentByClass<UEquipmentComponent>();
	if (EquipmentComponent == nullptr) return;
	EquipmentComponent->OnInventoryChanged.AddUObject(this, &UGearWidget::UpdateInventory);
	UpdateInventory(EquipmentComponent->GetStorageArray());
}

void UGearWidget::UpdateInventory(const TArray<FStorage>& StorageArray)
{
	if (StorageArray.Num() <= 0)
	{
		return;
	}
	
	StorageWidget->SetStorage(StorageArray[0]);
}