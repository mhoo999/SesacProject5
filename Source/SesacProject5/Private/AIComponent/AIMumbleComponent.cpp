// copyright by LootingEmployment


#include "AIComponent/AIMumbleComponent.h"

#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAIMumbleComponent::UAIMumbleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIMumbleComponent::BeginPlay()
{
	Super::BeginPlay();

	ai = Cast<ACharacterBase>(GetOwner());
}


// Called every frame
void UAIMumbleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIMumbleComponent::MultiRPCPlayCuriosityMumble_Implementation()
{
	if (ai)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Curiositymumble, ai->GetActorLocation(), ai->GetActorRotation());
	}
}

void UAIMumbleComponent::MultiRPCPlayAttackMumble_Implementation()
{
	if (ai)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attackmumble, ai->GetActorLocation(), ai->GetActorRotation());
	}
}

void UAIMumbleComponent::ServerRPCPlayPatrolMumble_Implementation()
{
	int32 randValue = FMath::RandRange(0,9);
	
	MultiRPCPlayPatrolMumble(randValue);
}

void UAIMumbleComponent::MultiRPCPlayPatrolMumble_Implementation(int32 randValue)
{
	if (randValue == 0)
	{
		mumbleSound = mumble1;
	}
	else if (randValue == 1)
	{
		mumbleSound = mumble2;
	}
	else if (randValue == 2)
	{
		mumbleSound = mumble3;
	}
	else
	{
		mumbleSound = nullptr;
	}

	if (mumbleSound != nullptr && ai != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mumbleSound, ai->GetActorLocation(), ai->GetActorRotation());
	}
}