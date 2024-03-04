// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageWidget.generated.h"

class UGridPanel;
class UItemWidget;
/**
 * 
 */
struct FStorage;
UCLASS()
class SESACPROJECT5_API UStorageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStorage(const FStorage& Storage);

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess, BindWidget))
	UGridPanel* GP_ItemSlot;
};