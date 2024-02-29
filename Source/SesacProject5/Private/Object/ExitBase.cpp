// copyright by LootingEmployment


#include "Object/ExitBase.h"

#include "Component/EscapeComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AExitBase::AExitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AExitBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AExitBase::OnCollisionComponentBeginOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AExitBase::OnCollisionComponentEndOverlap);
	}
}

// Called every frame
void AExitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExitBase::OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("AExitBase::OnComponentBeginOverlap) OtherActor Name :%s"), *OtherActor->GetActorNameOrLabel());
	if (UEscapeComponent* EscapeComponent = OtherActor->GetComponentByClass<UEscapeComponent>())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AExitBase::OnCollisionComponentBeginOverlap) %s ,%s, %s"), *GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetName());
		EscapeComponent->StartEscape(this);
	}
}

void AExitBase::OnCollisionComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UEscapeComponent* EscapeComponent = OtherActor->GetComponentByClass<UEscapeComponent>())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AExitBase::OnCollisionComponentEndOverlap) %s ,%s, %s"), *GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetName());
		EscapeComponent->EndEscape(this);
	}
}