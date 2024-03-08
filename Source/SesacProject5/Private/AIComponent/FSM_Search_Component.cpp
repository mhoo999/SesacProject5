// copyright by LootingEmployment


#include "AIComponent/FSM_Search_Component.h"

#include "AIComponent/AIMumbleComponent.h"
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
	ac->MoveToLocation(targetLastLoc, 50.f, true, true);
	if (!bCuriosityMumble)
	{
		bCuriosityMumble = true;
		auto mumbleComp = Cast<UAIMumbleComponent>(ai->GetComponentByClass<UAIMumbleComponent>());
		mumbleComp->MultiRPCPlayCuriosityMumble();
	}

	
	if (!bFind)
	{
		bFind = true;
		GetWorld()->GetTimerManager().ClearTimer(returnHandle);
		GetWorld()->GetTimerManager().SetTimer(returnHandle, this, &UFSM_Search_Component::AIReturnFunc, retrunDuration, false);
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

	ac->StopMovement();
	target = NewActor;
	targetLastLoc = NewActor->GetActorLocation();

	if (bFocusTarget())
	{
		ac->StopMovement();
		ac->SetContext(EEnemystate::chase);
		ac->GetFSM()->SenseNewActor(NewActor);
	}
}

void UFSM_Search_Component::AIReturnFunc()
{
	target = nullptr;
	ac->SetContext(EEnemystate::patrol);
	bFind = false;
	bCuriosityMumble = false;
}
