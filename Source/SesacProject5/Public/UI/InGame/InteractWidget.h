// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UInteractComponent;
class UVerticalBox;
class UTextBlock;
class IInteractInterface;
/**
 * 
 */
class UInteractionSlotWidget;
UCLASS()
class SESACPROJECT5_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

	void UpdateWidget(IInteractInterface* NewInteractActor);

	void SelectUp();
	void SelectDown();

	FText GetInteractionName() const;

	void StartInteract();
	void StopInteract();

private:
	UPROPERTY()
	UInteractComponent* InteractComponent;
	IInteractInterface* InteractActor;
	int32 SelectIndex;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_Name;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_InteractionSlot;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UInteractionSlotWidget> InteractionSlotWidgetClass;
};