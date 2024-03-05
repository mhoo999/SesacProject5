// copyright by LootingEmployment


#include "Object/QuestLocationBase.h"

#include "Components/BoxComponent.h"
#include "QuestSystem/ObjectiveComponent.h"
#include "QuestSystem/QuestLogComponent.h"

// Sets default values
AQuestLocationBase::AQuestLocationBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetHiddenInGame(false);
}

void AQuestLocationBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AQuestLocationBase::OnCollisionComponentBeginOverlap);
	}
}

void AQuestLocationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestLocationBase::OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UQuestLogComponent* questLogComp = OtherActor->GetComponentByClass<UQuestLogComponent>())
	{
		if (auto objectiveComp = GetComponentByClass<UObjectiveComponent>())
		{
			objectID = objectiveComp->GetObjectID();
			value = objectiveComp->GetValue();
			
			questLogComp->ClientRPCOnObjectiveIDCalled(objectID, value);
		}
	}
}

