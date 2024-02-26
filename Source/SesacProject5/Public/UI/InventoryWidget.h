// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"


class UVerticalBox;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInventoryComponent(UInventoryComponent* InventoryComponent);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_InventoryRow;
};