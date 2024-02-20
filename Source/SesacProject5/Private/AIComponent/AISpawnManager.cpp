// copyright by LootingEmployment


#include "AIComponent/AISpawnManager.h"

#include "AIController/EOSAIController.h"
#include "Character/ScavBase.h"

AAISpawnManager::AAISpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAISpawnManager::BeginPlay()
{
	Super::BeginPlay();

	MakeScave();
}

void AAISpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawnManager::MakeScave()
{
	if (HasAuthority())
	{
		FVector SpawnLoc = this->GetActorLocation();
		AScavBase* SpawnActor = GetWorld()->SpawnActor<AScavBase>(ScavFactory, SpawnLoc, FRotator::ZeroRotator);

		if (SpawnActor)
		{
			SpawnActor->GetController<AEOSAIController>()->SetWaypoint(waypointArray);
		}
	}
}

