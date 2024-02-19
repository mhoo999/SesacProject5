// copyright by LootingEmployment


#include "UI/InGame/EscapeWidget.h"

#include "Component/EscapeComponent.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UEscapeWidget::InitWidget(APawn* Pawn)
{
	if (UEscapeComponent* EscapeComponent = Pawn->GetComponentByClass<UEscapeComponent>())
	{
		EscapeComponent->OnIsEscapingChanged.BindUObject(this, &UEscapeWidget::UpdateIsEscaping);
		EscapeComponent->OnEscapeTimeChanged.BindUObject(this, &UEscapeWidget::UpdateEscapeTime);
	}
}

void UEscapeWidget::UpdateIsEscaping(bool bNewIsEscaping)
{
	if (bNewIsEscaping)
	{
		Brd_EscapeTime->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		Brd_EscapeTime->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEscapeWidget::UpdateEscapeTime(float NewEscapeTime)
{
	TB_EscapeTime->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), NewEscapeTime)));
}