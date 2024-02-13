// copyright by LootingEmployment


#include "Character/ScavBase.h"

#include "AIController/HoonsAIController.h"

AScavBase::AScavBase()
{
	static ConstructorHelpers::FClassFinder<AHoonsAIController> HoonsAIController(TEXT("/Game/YMH/Blueprint/BP_AIController_YMH.BP_AIController_YMH_C"));
	if (HoonsAIController.Succeeded())
	{
		AIControllerClass = HoonsAIController.Class;
	}
}
