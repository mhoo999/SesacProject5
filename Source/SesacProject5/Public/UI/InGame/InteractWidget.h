// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class IInteractInterface;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

	void UpdateWidget(IInteractInterface* NewInteractActor);
};