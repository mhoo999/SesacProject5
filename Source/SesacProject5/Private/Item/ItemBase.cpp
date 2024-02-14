// copyright by LootingEmployment


#include "Item/ItemBase.h"

#include <Kismet/GameplayStatics.h>

#include "Components/BoxComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::Interact(ACharacter* InteractCharacter, FString InteractionName)
{
	Destroy();
}

const TArray<FString>& AItemBase::GetInteractionNameArray() const
{
	return InteractionNameArray;
}

FText AItemBase::GetActorName() const
{
	return TempItemName;
}