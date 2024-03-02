// copyright by LootingEmployment


#include "AIComponent/FSM_Patrol_Component.h"

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

void UFSM_Patrol_Component::MultiRPCPerformLookAround_Implementation(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay && ai->GetMesh()->GetAnimInstance())
	{
		ai->GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		// UE_LOG(LogTemp, Warning, TEXT("play montage"));
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
			MultiRPCPerformLookAround(lookAroundMontage);
			bHasPerformedLookAround = true;
		}
		else
		{
			if (LookAroundTimerhandle.IsValid()) return;

			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimerhandle);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimerhandle, this, &UFSM_Patrol_Component::OnLookAtroundTimerExpired, lookAroundMontage->GetPlayLength(), false);
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

void UFSM_Patrol_Component::OnLookAtroundTimerExpired()
{
	bHasPerformedLookAround = false;
	SetNextDestination();
}

