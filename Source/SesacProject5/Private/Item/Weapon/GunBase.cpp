// copyright by LootingEmployment


#include "Item/Weapon/GunBase.h"

#include "AIController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
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

void AGunBase::OnRep_Owner()
{
	Super::OnRep_Owner();

	OwningCharacter = Cast<ACharacter>(Owner);
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
		
	}
}

void AGunBase::StartFire()
{
	if (HasAuthority())
	{
		ServerRPC_StartFire_Implementation();
	}
	else
	{
		ServerRPC_StartFire();	
	}
}

void AGunBase::StopFire()
{
	ServerRPC_StopFire();
	OwningCharacter->StopAnimMontage(FireMontage);
}

void AGunBase::FireBullet(FVector TargetLocation)
{
	if (HasAuthority())
	{
		ServerRPC_FireBullet_Implementation(TargetLocation, FireArrowComponent->GetComponentLocation());
	}
	else
	{
		ServerRPC_FireBullet(TargetLocation, FireArrowComponent->GetComponentLocation());
	}
}

void AGunBase::ServerRPC_FireBullet_Implementation(FVector TargetLocation, FVector FromLocation)
{
	FVector FireDirection = TargetLocation - FireArrowComponent->GetComponentLocation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AProjectileBase* Bullet = GetWorld()->SpawnActor<AProjectileBase>(BulletClass, FromLocation, FRotationMatrix::MakeFromX(FireDirection).Rotator(), Params))
	{
		Bullet->SetOwner(GetOwner());
	}
}

void AGunBase::MultiRPC_StartFire_Implementation()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGunBase::MultiRPC_StartFire_Implementation"));
	OwningCharacter->PlayAnimMontage(FireMontage);
}

void AGunBase::MultiRPC_StopFire_Implementation()
{
	if (OwningCharacter->IsLocallyControlled() == false && FireMontage)
    {
    	OwningCharacter->StopAnimMontage(FireMontage);
    }
}

void AGunBase::ServerRPC_StopFire_Implementation()
{
	
	if (false == bIsTriggered) return;
	bIsTriggered = false;
	MultiRPC_StopFire();
}

void AGunBase::ServerRPC_StartFire_Implementation()
{
	if (true == bIsTriggered) return;
	// UE_LOG(LogTemp, Warning, TEXT("AGunBase::ServerRPC_StartFire_Implementation"));
	bIsTriggered = true;
	MultiRPC_StartFire();
}