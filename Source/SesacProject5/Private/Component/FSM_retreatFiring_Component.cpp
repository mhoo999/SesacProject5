// copyright by LootingEmployment


#include "Component/FSM_retreatFiring_Component.h"

#include "AIController/HoonsAIController.h"

UFSM_retreatFiring_Component::UFSM_retreatFiring_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_retreatFiring_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UFSM_retreatFiring_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFSM_retreatFiring_Component::ExecuteBehavior()
{
}

void UFSM_retreatFiring_Component::StopExecute()
{
	ac->StopMovement();
}

void UFSM_retreatFiring_Component::SenseNewActor(AActor* NewActor)
{
}

