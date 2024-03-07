// copyright by LootingEmployment


#include "AIComponent/FSM_Patrol_Component.h"

#include "AIComponent/AIMontageComponent.h"
#include "AIComponent/AIMumbleComponent.h"
#include "AIController/EOSAIController.h"
#include "Character/CharacterBase.h"

UFSM_Patrol_Component::UFSM_Patrol_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Patrol_Component::BeginPlay()
{
	Super::BeginPlay();
}

bool UFSM_Patrol_Component::IsAtDestination()
{
	if (NextWaypoint == nullptr) return false;
	
	float dist = FVector::Dist(NextWaypoint->GetActorLocation(), ai->GetActorLocation());
	return dist <= 100;
}

void UFSM_Patrol_Component::PerformLookAround()
{
	auto montageComp = Cast<UAIMontageComponent>(ai->GetComponentByClass<UAIMontageComponent>());
	
	if (montageComp)
	{
		montageComp->MultiRPCPlayLookAround();
	}
}

void UFSM_Patrol_Component::SetNextDestination()
{
	if (!bHasNextWaypoint)
	{
		bHasNextWaypoint = true;
		NextWaypoint = waypointArray[CurrentWaypointIndex];
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % waypointArray.Num();
	}

	GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
}

void UFSM_Patrol_Component::MoveToDestination()
{
	ac->MoveToActor(NextWaypoint, AcceptanceRadius, true, true, false, 0, true);
}

void UFSM_Patrol_Component::ExecuteBehavior()
{
	if (waypointArray.Num() == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("waypoint list is empty"));
		return;
	}

	if (!bMumble)
	{
		bMumble = true;
		float duration = 6.f;
		
		auto mumbleComp = Cast<UAIMumbleComponent>(ai->GetComponentByClass<UAIMumbleComponent>());
		mumbleComp->ServerRPCPlayPatrolMumble();

		int32 randValue = FMath::RandRange(0,9);
		if (randValue == 0)
		{
			duration = 4.f;
		}
		else if (randValue == 1)
		{
			duration = 8.f;
		}
		else if (randValue == 2)
		{
			duration = 12.f;
		}
		
		GetWorld()->GetTimerManager().ClearTimer(mumbleTimerhandle);
		GetWorld()->GetTimerManager().SetTimer(mumbleTimerhandle, this, &UFSM_Patrol_Component::OnMumbleTimerExpired, duration, false);
	}
	
	NextWaypoint = waypointArray[CurrentWaypointIndex];
	
	if (IsAtDestination())
	{
		if (!bHasPerformedLookAround)
		{
			PerformLookAround();
			bHasPerformedLookAround = true;
		}
		else
		{
			if (LookAroundTimerhandle.IsValid()) return;

			auto montageComp = Cast<UAIMontageComponent>(ai->GetComponentByClass<UAIMontageComponent>());
			lookAroundTime = montageComp->getLookAtoundMontageTime();
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimerhandle, this, &UFSM_Patrol_Component::OnLookAtroundTimerExpired, lookAroundTime, false);
		}
	}
	else
	{
		bHasNextWaypoint = false;
		MoveToDestination();
	}
}

void UFSM_Patrol_Component::StopExecute()
{
	ac->StopMovement();
	GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
}

void UFSM_Patrol_Component::SenseNewActor(AActor* NewActor)
{
	if (NewActor == nullptr)
	{
		return;
	}

	ac->StopMovement();
	ac->SetContext(EEnemystate::search);
	ac->GetFSM()->SenseNewActor(NewActor);
}

void UFSM_Patrol_Component::OnLookAtroundTimerExpired()
{
	bHasPerformedLookAround = false;
	SetNextDestination();
}

void UFSM_Patrol_Component::OnMumbleTimerExpired()
{
	bMumble = false;
}

