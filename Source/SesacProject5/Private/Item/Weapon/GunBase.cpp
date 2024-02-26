// copyright by LootingEmployment


#include "Item/Weapon/GunBase.h"

#include "FPSAnim_AnimInstance.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "FPSAnim_CharacterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/ProjectileBase.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	WeaponComponent = OwningCharacter->GetComponentByClass<UWeaponComponent>(); 
}

void AGunBase::AttachToCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("AGunBase::AttachToCharacter) Debug 1"));
	if (UFPSAnim_CharacterComponent* FPSAnim_Character = OwningCharacter->GetComponentByClass<UFPSAnim_CharacterComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("AGunBase::AttachToCharacter) Debug 2"));
		AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FirearmGripSocket);
		FPSAnim_Character->SetCurrentFirearm(this);
		SetCharacterComponent(FPSAnim_Character);
	}
}

void AGunBase::DetachFromCharacter()
{
	
}

void AGunBase::Reload()
{
	ServerRPC_Reload();
	OnReload();
}

void AGunBase::OnReload()
{
	GetCharacterComponent()->GetAnimationInstance()->SetIsReloading(true);
	if (ReloadMontage_Character)
	{
		// GetCharacterComponent()->GetAnimationInstance()->Montage_Play(ReloadMontage_Character);
		GetCharacterComponent()->GetInUseMesh()->GetAnimInstance()->Montage_Play(ReloadMontage_Character);
	}
	if (ReloadMontage)
	{
		Cast<USkeletalMeshComponent>(FirearmMesh)->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
}

void AGunBase::ReloadComplete()
{
	GetCharacterComponent()->GetAnimationInstance()->SetIsReloading(false);
}

void AGunBase::StartAim()
{
	if (UFPSAnim_CharacterComponent* FPSAnim_Character = OwningCharacter->GetComponentByClass<UFPSAnim_CharacterComponent>())
	{
		FPSAnim_Character->StartAiming();
	}
}

void AGunBase::StopAim()
{
	if (UFPSAnim_CharacterComponent* FPSAnim_Character = OwningCharacter->GetComponentByClass<UFPSAnim_CharacterComponent>())
	{
		FPSAnim_Character->StopAiming();
	}
}

void AGunBase::ServerRPC_Reload_Implementation()
{
	MultiRPC_Reload();
}

void AGunBase::MultiRPC_Reload_Implementation()
{
	if (GetCharacterComponent()->IsLocallyControlled()) return;
	OnReload();
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunBase::StartFire()
{
	FireBullet(WeaponComponent->GetFocusLocation());
	// if (HasAuthority())
	// {
	// 	ServerRPC_StartFire_Implementation();
	// }
	// else
	// {
	// 	ServerRPC_StartFire();	
	// }
}

void AGunBase::StopFire()
{
	ServerRPC_StopFire();
	OwningCharacter->StopAnimMontage(FireMontage);
}

void AGunBase::FireBullet(FVector TargetLocation) 
{
	ServerRPC_FireBullet(TargetLocation, GetMuzzleSocketTransform().GetLocation());
	PerformProceduralRecoil();
	if (FireSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetMuzzleSocketTransform().GetLocation(), GetMuzzleSocketTransform().GetRotation().Rotator(), 3);
}

void AGunBase::MultiRPC_FireBullet_Implementation(FVector FromLocation)
{
	if (GetCharacterComponent()->IsLocallyControlled()) return;

	PerformProceduralRecoil();
	if (FireSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, FromLocation, GetMuzzleSocketTransform().GetRotation().Rotator(), 3);
}

void AGunBase::ServerRPC_FireBullet_Implementation(FVector TargetLocation, FVector FromLocation)
{
	// FVector FireDirection = CurrentSightComponent ? SightComponents[SightComponentIndex]->GetRightVector() : TargetLocation - FromLocation;
	// Todo : Use Sight Right Vector, But how verify is ai or player ? (AI must use to targetlocation)
	FVector FireDirection = TargetLocation - FromLocation;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AProjectileBase* Bullet = GetWorld()->SpawnActor<AProjectileBase>(BulletClass, FromLocation, FRotationMatrix::MakeFromX(FireDirection).Rotator(), Params))
	{
		Bullet->SetOwner(GetOwner());
	}
	
	MultiRPC_FireBullet(FromLocation);
}

void AGunBase::MultiRPC_StartFire_Implementation()
{
}

void AGunBase::MultiRPC_StopFire_Implementation()
{
}

void AGunBase::ServerRPC_StopFire_Implementation()
{
}

void AGunBase::ServerRPC_StartFire_Implementation()
{
}