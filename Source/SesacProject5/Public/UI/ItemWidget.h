// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

struct FItemInstance;
class USizeBox;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FItemInstance& ItemInstance);
	
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	USizeBox* SB_WidgetSize;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_ItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_ItemStackCount;
};