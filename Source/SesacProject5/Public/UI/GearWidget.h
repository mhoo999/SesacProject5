// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GearWidget.generated.h"

class UStorageWidget;
struct FStorage;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UGearWidget : public UUserWidget
{
	GENERATED_BODY()

	void Init(APawn* Pawn);

	void UpdateInventory(TArray<FStorage>& StorageArray);
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UStorageWidget* StorageWidget;
};