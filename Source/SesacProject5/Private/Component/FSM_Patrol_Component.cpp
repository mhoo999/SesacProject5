// copyright by LootingEmployment


#include "Component/FSM_Patrol_Component.h"

#include "NavigationSystem.h"
#include "AIController/HoonsAIController.h"
#include "Navigation/PathFollowingComponent.h"

UFSM_Patrol_Component::UFSM_Patrol_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM_Patrol_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UFSM_Patrol_Component::ExecuteBehavior(AAIController* aiController, APawn* ai)
{
	FPathFollowingRequestResult result;
	result.Code = aiController->MoveToLocation(RandomLocation);
	if (result != EPathFollowingRequestResult::RequestSuccessful)
	{
		UpdateRandomLocation(ai->GetActorLocation(), 500, RandomLocation);
	}
}

bool UFSM_Patrol_Component::UpdateRandomLocation(FVector origin, float radius, FVector& outLocation) const
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(origin, radius, loc);

	if (result)
	{
		outLocation = loc.Location;
		return true;
	}

	return false;
}
