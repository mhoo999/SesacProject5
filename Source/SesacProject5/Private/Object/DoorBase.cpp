// copyright by LootingEmployment


#include "Object/DoorBase.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Interact(ACharacter* InteractCharacter, FText InteractionName)
{
	FString Interaction = InteractionName.ToString();
	if (Interaction.Equals("Open"))
	{
	}
}

const TArray<FText>& ADoorBase::GetInteractionNameArray() const
{
	return InteractionNameArray;
}

FText ADoorBase::GetActorName() const
{
	return FText::FromName("Door");
}