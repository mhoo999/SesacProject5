// copyright by LootingEmployment


#include "UI/InventoryWidget.h"
#include <Component/InventoryComponent.h>

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "UI/StorageWidget.h"

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* InventoryComponent)
{
	TArray<FStorage>& StorageArray = InventoryComponent->GetStorageArray();
	int32 StorageIndex = 0;
	
	TArray<UWidget*> InventoryRowArray = VB_InventoryRow->GetAllChildren();
	
	for (int i = 0; i < InventoryRowArray.Num(); ++i)
	{
		if (UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(InventoryRowArray[i]))
		{
			TArray<UWidget*> ItemColumnArray = HorizontalBox->GetAllChildren();

			for (int j = 0; j < ItemColumnArray.Num(); ++j)
			{
				if (UStorageWidget* StorageWidget = Cast<UStorageWidget>(ItemColumnArray[j]))
				{
					StorageWidget->SetStorage(StorageArray[StorageIndex++]);
				}
				else if (UVerticalBox* InventorySlotArray = Cast<UVerticalBox>(ItemColumnArray[i]))
				{
					
				}
			}
		}
	}
}
