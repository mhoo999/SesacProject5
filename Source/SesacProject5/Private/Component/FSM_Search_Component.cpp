// copyright by LootingEmployment


#include "Component/FSM_Search_Component.h"

#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"

UFSM_Search_Component::UFSM_Search_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM_Search_Component::BeginPlay()
{
	Super::BeginPlay();

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());
}

void UFSM_Search_Component::ExecuteBehavior()
{
	ac->MoveToLocation(dest, 50.f, true, true);
}

void UFSM_Search_Component::StopExecute()
{
	ac->StopMovement();
}

void UFSM_Search_Component::SenseNewActor(AActor* NewActor)
{
	if (NewActor == nullptr)
	{
		ac->SetContext(EEnemystate::patrol);
	}
	
	dest = NewActor->GetActorLocation();
	
	// newActor의 TeamId가 본인의 TeamId와 다를 경우, 상태를 Chase로 변환
	ACharacterBase* chr = Cast<ACharacterBase>(NewActor);
	if (ai->TeamId != chr->TeamId && chr->TeamId != 255)
	{
		ac->SetContext(EEnemystate::chase);
		ac->GetFSM()->SenseNewActor(NewActor);
	}
}
