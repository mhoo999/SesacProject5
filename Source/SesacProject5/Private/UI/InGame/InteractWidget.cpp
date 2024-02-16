// copyright by LootingEmployment


#include "UI/InGame/InteractWidget.h"

#include "Component/InteractComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Interface/InteractInterface.h"
#include "UI/InGame/InteractionSlotWidget.h"

void UInteractWidget::InitWidget(APawn* Pawn)
{
	if (UInteractComponent* InteractComponent = Pawn->GetComponentByClass<UInteractComponent>())
	{
		InteractComponent->SetInteractWidget(this);
		InteractComponent->OnInteractActorChanged.BindUObject(this, &UInteractWidget::UpdateWidget);
	}
}

void UInteractWidget::UpdateWidget(IInteractInterface* NewInteractActor)
{
	
	if (NewInteractActor == nullptr)
	{ 
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	TB_Name->SetText(NewInteractActor->GetActorName());
	VB_InteractionSlot->ClearChildren();
	
	const TArray<FText>& InteractionNameArray = NewInteractActor->GetInteractionNameArray();

	for (FText Iter : InteractionNameArray)
	{
		UInteractionSlotWidget* InteractionSlotWidget = CreateWidget<UInteractionSlotWidget>(GetOwningPlayer(), InteractionSlotWidgetClass);
		InteractionSlotWidget->InitWidget(Iter);
		VB_InteractionSlot->AddChildToVerticalBox(InteractionSlotWidget);
	}
	
	SelectIndex = 0;
	Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->Focus();
}

void UInteractWidget::SelectUp()
{
	if (SelectIndex == 0) return;

	Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex--))->Unfocus();
	Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->Focus();
}

void UInteractWidget::SelectDown()
{
	if (SelectIndex == VB_InteractionSlot->GetChildrenCount() - 1) return;

	Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex++))->Unfocus();
	Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->Focus();
}

FText UInteractWidget::GetInteractionName() const
{
	return Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->GetInteractionName();
}