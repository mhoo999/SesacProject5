// copyright by LootingEmployment


#include "Component/FSM_Patrol_Component.h"

#include "AIComponent/AIPatrolWaypoint.h"
#include "AIController/HoonsAIController.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

UFSM_Patrol_Component::UFSM_Patrol_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Patrol_Component::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolWaypoint::StaticClass(), waypointArray);
}

void UFSM_Patrol_Component::ExecuteBehavior()
{
	if (waypointArray.Num() == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("waypoint list is empty"));
		return;
	}
	
	AActor* NextWaypoint = waypointArray[CurrentWaypointIndex];

	if (NextWaypoint && ac)
	{
		ac->MoveToActor(NextWaypoint, AcceptanceRadius, true, true, false, 0, true);

		float dist = FVector::Dist(NextWaypoint->GetActorLocation(), ai->GetActorLocation());
		if (dist <= 100)
		{
			CurrentWaypointIndex = (CurrentWaypointIndex + 1) % waypointArray.Num();
			UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentWaypointIndex);
		}
	}
}

void UFSM_Patrol_Component::StopExecute()
{
	ac->StopMovement();
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

