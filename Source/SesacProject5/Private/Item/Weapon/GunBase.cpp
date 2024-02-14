// copyright by LootingEmployment


#include "Item/Weapon/GunBase.h"

#include "Components/ArrowComponent.h"
#include "Projectile/ProjectileBase.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	FireArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("FireArrowComponent"));
	FireArrowComponent->SetupAttachment(SkeletalMeshComponent);
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetActorTickEnabled(true);

		CurrentFireCoolDown = FireCoolDown;
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentFireCoolDown < FireCoolDown) CurrentFireCoolDown += DeltaTime;

	if (bIsTriggered && CurrentFireCoolDown >= FireCoolDown)
	{
		// Todo : Check Ammo
		CurrentFireCoolDown -= FireCoolDown;
		
		// Todo : Fire Bullet;
		if (BulletClass) AProjectileBase* Bullet = GetWorld()->SpawnActor<AProjectileBase>(BulletClass, FireArrowComponent->GetComponentLocation(), FireArrowComponent->GetComponentRotation());
	}
}

void AGunBase::StartFire()
{
	ServerRPC_Fire();
}

void AGunBase::StopFire()
{
	ServerRPC_StopFire();
}

void AGunBase::ServerRPC_StopFire_Implementation()
{
	bIsTriggered = false;
}

void AGunBase::ServerRPC_Fire_Implementation()
{
	bIsTriggered = true;
}