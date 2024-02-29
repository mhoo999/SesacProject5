// copyright by LootingEmployment


#include "Object/QuestLocationBase.h"

#include "Components/BoxComponent.h"
#include "QuestSystem/QuestLogComponent.h"

// Sets default values
AQuestLocationBase::AQuestLocationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AQuestLocationBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AQuestLocationBase::OnCollisionComponentBeginOverlap);
	}
}

// Called every frame
void AQuestLocationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestLocationBase::OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));

	if (UQuestLogComponent* questLogComp = OtherActor->GetComponentByClass<UQuestLogComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap success"));
		questLogComp->ClientRPCOnObjectiveIDCalled(objectID, value);
	}
}

