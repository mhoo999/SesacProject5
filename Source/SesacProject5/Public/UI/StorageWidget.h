// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageWidget.generated.h"

/**
 * 
 */
struct FStorage;
UCLASS()
class SESACPROJECT5_API UStorageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStorage(FStorage& Storage);
};
