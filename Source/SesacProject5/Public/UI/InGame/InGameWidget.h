// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UHurtWidget;
class UEscapeWidget;
class UHealthWidget;
class UInteractWidget;
class UWidgetSwitcher;
class UTabWidget;
class UHurtWidget;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

	void ToggleTabWidget();

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UInteractWidget* InteractWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTabWidget* TabWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHealthWidget* HealthWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHurtWidget* HurtWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UEscapeWidget* EscapeWidget;
	

	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
};