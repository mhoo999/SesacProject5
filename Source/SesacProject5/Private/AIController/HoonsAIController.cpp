// copyright by LootingEmployment


#include "AIController/HoonsAIController.h"
#include "Component/FSM_Patrol_Component.h"

AHoonsAIController::AHoonsAIController()
{
	FSMPatrolComp = CreateDefaultSubobject<UFSM_Patrol_Component>(TEXT("Patrol Component"));
}

void AHoonsAIController::BeginPlay()
{
	Super::BeginPlay();

	ai = Cast<APawn>(GetPawn());
	FSMInterface = Cast<IFSMInterface>(FSMPatrolComp);
	check(ai && FSMInterface);
}

void AHoonsAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ai)
	{
		FSMInterface->ExecuteBehavior(this, ai);	
	}
}
