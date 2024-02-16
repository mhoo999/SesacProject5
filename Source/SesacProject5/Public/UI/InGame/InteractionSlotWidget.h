// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionSlotWidget.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UInteractionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(FText InteractionName);
	void Focus();
	void Unfocus();
	FText GetInteractionName() const;
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Border;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_InteractionName;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FLinearColor FocusColor;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FLinearColor UnfocusColor;
};
