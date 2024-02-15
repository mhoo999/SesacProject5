// copyright by LootingEmployment


#include "Component/FSM_Search_Component.h"

UFSM_Search_Component::UFSM_Search_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM_Search_Component::BeginPlay()
{
	Super::BeginPlay();

}

void UFSM_Search_Component::ExecuteBehavior(AActor* target)
{
}

void UFSM_Search_Component::StopExecute()
{
}

void UFSM_Search_Component::SenseNewActor(AActor* NewActor)
{
}
