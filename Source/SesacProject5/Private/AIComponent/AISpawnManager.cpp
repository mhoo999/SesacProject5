// copyright by LootingEmployment


#include "AIComponent/AISpawnManager.h"

// Sets default values
AAISpawnManager::AAISpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAISpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAISpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

