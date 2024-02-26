// copyright by LootingEmployment


#include "UI/InGame/InteractWidget.h"

#include "Component/InteractComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Interface/InteractInterface.h"
#include "UI/InGame/InteractionSlotWidget.h"

void UInteractWidget::InitWidget(APawn* Pawn)
{
	InteractComponent = Pawn->GetComponentByClass<UInteractComponent>();
	if (InteractComponent)
	{
		InteractComponent->SetInteractWidget(this);
		InteractComponent->OnInteractActorChanged.BindUObject(this, &UInteractWidget::UpdateWidget);
	}
}

void UInteractWidget::UpdateWidget(IInteractInterface* NewInteractActor)
{
	
	if (NewInteractActor == nullptr)
	{
		if (InteractActor) InteractActor->OnInteractActorChanged.Unbind();
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	InteractActor = NewInteractActor;
	InteractActor->OnInteractActorChanged.BindUObject(this, &UInteractWidget::UpdateWidget);
	
	SetVisibility(ESlateVisibility::Visible);
	TB_Name->SetText(NewInteractActor->GetActorName());
	VB_InteractionSlot->ClearChildren();
	
	const TArray<FText>& InteractionNameArray = NewInteractActor->GetInteractionNameArray();

	if (InteractionNameArray.Num() > 0)
	{
		for (FText Iter : InteractionNameArray)
		{
			UInteractionSlotWidget* InteractionSlotWidget = CreateWidget<UInteractionSlotWidget>(GetOwningPlayer(), InteractionSlotWidgetClass);
			InteractionSlotWidget->InitWidget(Iter);
			VB_InteractionSlot->AddChildToVerticalBox(InteractionSlotWidget);
		}
	
		SelectIndex = 0;
		Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->Focus();	
	}
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
	if (VB_InteractionSlot->GetChildrenCount() == 0) return FText();
	
	return Cast<UInteractionSlotWidget>(VB_InteractionSlot->GetChildAt(SelectIndex))->GetInteractionName();
}

void UInteractWidget::StartInteract()
{
	if (InteractComponent)
	{
		InteractComponent->StartInteraction();
	}
}

void UInteractWidget::StopInteract()
{
	if (InteractComponent)
	{
		InteractComponent->StopInteraction();
	}
}
