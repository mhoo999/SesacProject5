// copyright by LootingEmployment


#include "Projectile/ProjectileBase.h"

#include "Component/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	SetRootComponent(CollisionComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Velocity = FVector(3000.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// SetLifeSpan(1.f);
		SetReplicates(true);

		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCollisionComponentBeginOverlap);
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("AProjectileBase::OnCollisionComponentBeginOverlap"));
	if (UHealthComponent* HealthComponent = OtherActor->GetComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyDamage(this, SweepResult.BoneName);
	}

	Destroy();
}

EDamageType AProjectileBase::GetDamageType() const
{
	return EDamageType::Bullet;
}

float AProjectileBase::GetDamage() const
{
	return 10.f;
}

AActor* AProjectileBase::GetIndicator() const
{
	return GetOwner();
}