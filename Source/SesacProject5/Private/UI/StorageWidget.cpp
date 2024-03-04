// copyright by LootingEmployment


#include "UI/StorageWidget.h"

#include "Component/InventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "UI/ItemWidget.h"

void UStorageWidget::SetStorage(const FStorage& Storage)
{
	// UE_LOG(LogTemp, Error, TEXT("UStorageWidget::SetStorage"));
	if (ItemWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UStorageWidget::SetStorage) ItemWidgetClass is nullptr"));
		return;
	}
	GP_ItemSlot->ClearChildren();
	
	for (const FItemInstance& Iter : Storage.ItemArray)
	{
		// UE_LOG(LogTemp, Error, TEXT("UStorageWidget::SetStorage) Item Name : %s"), *Iter.ItemRow.ToString());
		// for (auto& ItemSlot : GP_ItemSlot->GetAllChildren())
		// {
		// 	if (Cast<UGridSlot>(ItemSlot)->GetRow() != Iter.StoragePosition.Y) return;
		// 	if (Cast<UGridSlot>(ItemSlot)->GetColumn() != Iter.StoragePosition.X) return;
		//
		// 	Cast<UPanelSlot>(ItemSlot)->;
		// }
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
		ItemWidget->Init(Iter);
		UGridSlot* GridSlot = GP_ItemSlot->AddChildToGrid(ItemWidget, Iter.StoragePosition.Y, Iter.StoragePosition.X);
	}
}