// copyright by LootingEmployment


#include "UI/ItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameInstance/EFSGameInstance.h"
#include "Item/ItemBase.h"

void UItemWidget::Init(const FItemInstance& ItemInstance)
{
	FItemData* ItemData = GetGameInstance<UEFSGameInstance>()->GetItemData(ItemInstance.ItemRow);
	if (ItemData == nullptr) return;

	Img_ItemImage->SetBrushFromTexture(ItemData->Image);
	TB_ItemStackCount->SetText(FText::AsNumber(ItemInstance.CurrentStack));
}