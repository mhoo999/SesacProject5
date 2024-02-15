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

	ac = Cast<AHoonsAIController>(GetOwner());
	ai = Cast<ACharacterBase>(ac->GetPawn());
}

void UFSM_Patrol_Component::ExecuteBehavior(AActor* target)
{
	if (waypointArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("waypoint list is empty"));
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
	ac->SetContext(EEnemystate::chase);
}

