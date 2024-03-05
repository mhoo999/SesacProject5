// copyright by LootingEmployment


#include "QuestSystem/ObjectiveComponent.h"

#include "Component/HealthComponent.h"
#include "QuestSystem/QuestLogComponent.h"

// Sets default values for this component's properties
UObjectiveComponent::UObjectiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UObjectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObjectiveComponent::OnReturnObjectID(bool bNewIsDead)
{
	if (bNewIsDead)
	{
		if (auto returnTarget = healthComp->GetAttackActor())
		{
			if (auto questLogComp = returnTarget->GetComponentByClass<UQuestLogComponent>())
			{
				questLogComp->ClientRPCOnObjectiveIDCalled(objectID, value);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UObjectiveComponent::OnReturnObjectID) QuestLogComp is not valid"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UObjectiveComponent::OnReturnObjectID) ReturnTarget is not valid"));
		}
	}
}

FString UObjectiveComponent::GetObjectID()
{
	return objectID;
}

int32 UObjectiveComponent::GetValue()
{
	return value;
}

void UObjectiveComponent::SetObjectID(FString ID)
{
	objectID = ID;
}

void UObjectiveComponent::SetValue(int32 integer)
{
	value = integer;
}

void UObjectiveComponent::SetObserveHealth()
{
	healthComp = GetOwner()->GetComponentByClass<UHealthComponent>();
	healthComp->OnIsDeadChanged.AddUObject(this, &UObjectiveComponent::OnReturnObjectID);
}