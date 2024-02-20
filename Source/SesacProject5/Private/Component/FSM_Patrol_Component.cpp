// copyright by LootingEmployment


#include "Component/FSM_Patrol_Component.h"

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

void UFSM_Patrol_Component::PerfomLookAround()
{
	ai->GetMesh()->GetAnimInstance()->Montage_Play(lookAroundMontage);
}

void UFSM_Patrol_Component::SetNextDestination()
{
	if (!bHasNextWaypoint)
	{
		bHasNextWaypoint = true;
		NextWaypoint = waypointArray[CurrentWaypointIndex];
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % waypointArray.Num();
	}
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

	NextWaypoint = waypointArray[CurrentWaypointIndex];
	
	if (IsAtDestination())
	{
		if (!bHasPerformedLookAround)
		{
			PerfomLookAround();
			bHasPerformedLookAround = true;
		}
		else
		{
			if (LookAroundTimerhandle.IsValid()) return;

			
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimerhandle, FTimerDelegate::CreateLambda([&]
			{
				GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
				bHasPerformedLookAround = false;
				SetNextDestination();
			}), lookAroundMontage->GetPlayLength(), false, lookAroundMontage->GetPlayLength());
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
	
	ac->SetContext(EEnemystate::search);
	ac->GetFSM()->SenseNewActor(NewActor);
}

