// copyright by LootingEmployment


#include "Character/ScavBase.h"

#include "AIComponent/AIMontageComponent.h"
#include "AIComponent/AIMumbleComponent.h"
#include "AIController/EOSAIController.h"
#include "QuestSystem/ObjectiveComponent.h"

AScavBase::AScavBase()
{
	static ConstructorHelpers::FClassFinder<AEOSAIController> EOSAIController(TEXT("/Game/YMH/Blueprint/Controller/BP_AIController_YMH.BP_AIController_YMH_C"));
	if (EOSAIController.Succeeded())
	{
		AIControllerClass = EOSAIController.Class;
	}

	TeamId = FGenericTeamId(1);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIMontageComp = CreateDefaultSubobject<UAIMontageComponent>(TEXT("Montage Component"));
	AIMumbleComp = CreateDefaultSubobject<UAIMumbleComponent>(TEXT("Mumble Component"));
	ObjectiveComp = CreateDefaultSubobject<UObjectiveComponent>(TEXT("Objective Component"));

	ObjectiveComp->SetObserveHealth();
}
