// copyright by LootingEmployment


#include "Projectile/ProjectileBase.h"

#include "Component/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DecalActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CollisionComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Velocity = FVector(3000.f, 0.f, 0.f);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
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
	UE_LOG(LogTemp, Warning, TEXT("AProjectileBase::OnCollisionComponentBeginOverlap) %s, %s, %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetName(), *SweepResult.BoneName.ToString());
	
	if (OtherActor == GetOwner()) return;
	
	MultRPC_SpawnBulletDecal(OtherActor, SweepResult.Location, FRotationMatrix::MakeFromX(SweepResult.ImpactNormal).Rotator());

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
	return 58.f;
}

AActor* AProjectileBase::GetIndicator() const
{
	return GetOwner();
}

void AProjectileBase::Init(AActor* NewOwner)
{
	SetOwner(NewOwner);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AProjectileBase::MultRPC_SpawnBulletDecal_Implementation(AActor* HitActor, FVector SpawnLocation, FRotator SpawnRotation)
{
	FName Material;
	if (HitActor->ActorHasTag(FName("Flesh")))
	{
		Material = FName("Flesh");
	}
	else
	{
		Material = FName("Other");
	}

	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMap[Material], FVector(5,5,5)
    							, SpawnLocation, SpawnRotation);
    		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterMap[Material], SpawnLocation, SpawnRotation);
    		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundMap[Material], SpawnLocation, SpawnRotation);
}
