// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "AmmoBox.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AAmmoBox : public AItemBase
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacter* InteractCharacter, FText InteractionName) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	int32 AmmoCount = 30;
};