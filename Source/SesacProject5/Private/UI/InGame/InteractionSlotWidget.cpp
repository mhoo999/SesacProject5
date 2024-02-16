// copyright by LootingEmployment


#include "UI/InGame/InteractionSlotWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UInteractionSlotWidget::InitWidget(FText InteractionName)
{
	TB_InteractionName->SetText(InteractionName);
}

void UInteractionSlotWidget::Focus()
{
	Border->SetBrushColor(FocusColor);
	TB_InteractionName->SetColorAndOpacity(FSlateColor(FColor::Black));
}

void UInteractionSlotWidget::Unfocus()
{
	Border->SetBrushColor(UnfocusColor);
	TB_InteractionName->SetColorAndOpacity(FSlateColor(FColor::White));
}

FText UInteractionSlotWidget::GetInteractionName() const
{
	return TB_InteractionName->GetText();
}
