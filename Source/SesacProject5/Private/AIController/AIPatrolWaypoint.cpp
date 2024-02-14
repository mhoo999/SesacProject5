// copyright by LootingEmployment


#include "AIController/AIPatrolWaypoint.h"

AAIPatrolWaypoint::AAIPatrolWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Component"));
	RootComponent = VisualComponent;
}

void AAIPatrolWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIPatrolWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

