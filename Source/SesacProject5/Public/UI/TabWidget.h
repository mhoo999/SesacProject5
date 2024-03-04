// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabWidget.generated.h"

class UGearWidget;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UTabWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void InitWidget(APawn* Pawn);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SwitchToGearWidget();

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UGearWidget* GearWidget;

	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UButton* Btn_Tasks;
};