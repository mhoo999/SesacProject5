// copyright by LootingEmployment


#include "Component/FSM_Patrol_Component.h"

#include "AIController/AIPatrolWaypoint.h"
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

void UFSM_Patrol_Component::ExecutePatrol()
{
	if (waypointArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("waypoint list is empty"));
		return;
	}

	auto aiController = Cast<AHoonsAIController>(GetOwner());
	auto ai = Cast<ACharacterBase>(aiController->GetPawn());
	
	AActor* NextWaypoint = waypointArray[CurrentWaypointIndex];

	if (NextWaypoint && aiController)
	{
		aiController->MoveToActor(NextWaypoint, AcceptanceRadius, true, true, false, 0, true);

		float dist = FVector::Dist(NextWaypoint->GetActorLocation(), ai->GetActorLocation());
		if (dist <= 100)
		{
			CurrentWaypointIndex = (CurrentWaypointIndex + 1) % waypointArray.Num();
			UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentWaypointIndex);
		}
	}
}

void UFSM_Patrol_Component::ExecuteSearch()
{
}

void UFSM_Patrol_Component::ExecuteChase()
{
}

void UFSM_Patrol_Component::ExecuteAttack()
{
}

void UFSM_Patrol_Component::ExecuteRetreatFiring()
{
}

void UFSM_Patrol_Component::ExecuteAdvanceFiring()
{
}

void UFSM_Patrol_Component::ExecuteEvade()
{
}

void UFSM_Patrol_Component::ExecuteCamping()
{
}

void UFSM_Patrol_Component::ExecuteSelfHealing()
{
}
