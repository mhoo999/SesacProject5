// copyright by LootingEmployment


#include "UI/InGame/InteractWidget.h"

#include "Component/InteractComponent.h"
#include "Interface/InteractInterface.h"

void UInteractWidget::InitWidget(APawn* Pawn)
{
	if (UInteractComponent* InteractComponent = Pawn->GetComponentByClass<UInteractComponent>())
	{
		InteractComponent->OnInteractActorChanged.BindUObject(this, &UInteractWidget::UpdateWidget);
	}
}

void UInteractWidget::UpdateWidget(IInteractInterface* NewInteractActor)
{
	
	if (NewInteractActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractWidget::UpdateWidget) Debug 1"));
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UInteractWidget::UpdateWidget) Debug 2"));
	SetVisibility(ESlateVisibility::Visible);
	NewInteractActor->GetActorName();
	NewInteractActor->GetInteractionNameArray();
}