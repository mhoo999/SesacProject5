// copyright by LootingEmployment


#include "Component/FSM_Search_Component.h"

#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"

UFSM_Search_Component::UFSM_Search_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM_Search_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UFSM_Search_Component::ExecuteBehavior()
{
	if (target == nullptr) return;
	
	// 감지된 구역으로 이동
	targetLastLoc = target->GetActorLocation();
	ac->MoveToLocation(targetLastLoc, 50.f, true, true);
	
	// newActor의 TeamId가 본인의 TeamId와 다를 경우, Chase 상태로 전환
	ACharacterBase* chr = Cast<ACharacterBase>(target);
	if (ai->TeamId != chr->TeamId && chr->TeamId != 255)
	{
		ac->SetContext(EEnemystate::chase);
		ac->GetFSM()->SenseNewActor(target);
	}
	else // Enemy가 아닐 경우, Patrol 상태로 전환
	{
		ac->SetContext(EEnemystate::patrol);
		ac->GetFSM()->SenseNewActor(nullptr);
	}
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

	target = NewActor;
}
