// copyright by LootingEmployment


#include "UI/StorageWidget.h"

#include "Component/InventoryComponent.h"
#include "Components/GridPanel.h"
#include "UI/ItemWidget.h"

void UStorageWidget::SetStorage(FStorage& Storage)
{
	for (FItemInstance& Iter : Storage.ItemArray)
	{
		// for (auto& ItemSlot : GP_ItemSlot->GetAllChildren())
		// {
		// 	if (Cast<UGridSlot>(ItemSlot)->GetRow() != Iter.StoragePosition.Y) return;
		// 	if (Cast<UGridSlot>(ItemSlot)->GetColumn() != Iter.StoragePosition.X) return;
		//
		// 	Cast<UPanelSlot>(ItemSlot)->;
		// }
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
		ItemWidget->Init(Iter);
		GP_ItemSlot->AddChildToGrid(ItemWidget, Iter.StoragePosition.Y, Iter.StoragePosition.X);
	}
}