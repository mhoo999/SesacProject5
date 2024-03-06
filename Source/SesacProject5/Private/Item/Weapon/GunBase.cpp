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

float AGunBase::GetRecoilPitch() const
{
	return RecoilPitch;
}

float AGunBase::GetRecoilYaw() const
{
	return RecoilYaw;
}

void AGunBase::AddRecoil()
{
	RecoilPitch = FMath::Max(RecoilPitchMax, RecoilPitch + FMath::RandRange(-1.f, -3.f));
	RecoilYaw += FMath::RandRange(-3.f, 3.f);
}

void AGunBase::ToggleFireMode()
{
}

bool AGunBase::IsAttacking() const
{
	return false;
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

	if (RecoilPitch >= 0.f)
	{
		RecoilPitch = FMath::Max(RecoilPitch - DeltaTime * 5.f, 0.f);
	}
	if (RecoilYaw >= 0.f)
	{
		RecoilYaw = FMath::Max(RecoilYaw - DeltaTime * 5.f, 0.f);
	}
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
	{
		// Recoil
		// OwningCharacter->AddControllerPitchInput(FMath::RandRange(-1.f, -5.f));
		// OwningCharacter->AddControllerYawInput(FMath::RandRange(-5.f, 5.f));
		//
		// OwningCharacter->GetController<APlayerController>()->RotationInput.Pitch = OwningCharacter->GetController<APlayerController>()->RotationInput.Pitch = 10.f;
		// OwningCharacter->GetController<APlayerController>()->RotationInput.Pitch = OwningCharacter->GetController<APlayerController>()->RotationInput.Yaw = 10.f;
	}
	
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