// copyright by LootingEmployment


#include "AIController/HoonsAIController.h"

#include "Component/FSMComponent.h"

void AHoonsAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		auto FSMcomp = Cast<UFSMComponent>(ControlledPawn->GetComponentByClass(UFSMComponent::StaticClass()));

		if (FSMcomp)
		{
			IFSMInterface* FSM = Cast<IFSMInterface>(FSMcomp);
			if (FSM)
			{
				FSM->ExecuteBehavior();
			}
		}
	}
}
