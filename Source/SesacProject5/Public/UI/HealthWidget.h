// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UHealthSlotWidget;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

	void TestFunc(float, float);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* HeadSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* ThoraxSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* StomachSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* LeftArmSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* RightArmSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* LeftLegSlot;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthSlotWidget* RightLegSlot;
};
