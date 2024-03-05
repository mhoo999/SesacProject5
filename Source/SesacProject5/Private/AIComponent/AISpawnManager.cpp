// copyright by LootingEmployment


#include "AIComponent/AISpawnManager.h"

#include "AIController/EOSAIController.h"
#include "Character/ScavBase.h"
#include "Component/HealthComponent.h"

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
		GetWorld()->GetTimerManager().ClearTimer(respawnTimer);
		FVector SpawnLoc = this->GetActorLocation();
		AScavBase* SpawnActor = GetWorld()->SpawnActor<AScavBase>(ScavFactory, SpawnLoc, FRotator::ZeroRotator);

		if (SpawnActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Scav!"));
			SpawnActor->GetController<AEOSAIController>()->SetWaypoint(waypointArray);
			SpawnActor->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &AAISpawnManager::RespawnScave);
		}
	}
}

void AAISpawnManager::RespawnScave(bool bNewIsDead)
{
	if (bNewIsDead)
	{
		GetWorld()->GetTimerManager().SetTimer(respawnTimer, this, &AAISpawnManager::MakeScave, respawnTime, false);
	}
}

