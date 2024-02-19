// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UEscapeWidget;
class UHealthWidget;
class UInteractWidget;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UInteractWidget* InteractWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthWidget* HealthWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UEscapeWidget* EscapeWidget;
};