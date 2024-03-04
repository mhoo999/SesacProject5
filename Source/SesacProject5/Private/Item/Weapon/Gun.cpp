// copyright by LootingEmployment


#include "Item/Weapon/Gun.h"

#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile/ProjectileBase.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);
	AimOffset = CreateDefaultSubobject<USceneComponent>(TEXT("AimOffset"));
	AimOffset->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}


	// RecoilTimeline Setting
	if (ControllerRecoilCurve != nullptr)
	{
		FOnTimelineFloat CurveCallback;
		CurveCallback.BindUFunction(this, FName("ControllerRecoil"));

		ControllerRecoilTimeline.AddInterpFloat(ControllerRecoilCurve, CurveCallback);
		ControllerRecoilTimeline.SetTimelineLength(0.15f);
	}

	// AimTimeline Setting
	if (AimCurve != nullptr)
	{
		FOnTimelineFloat AimCurveCallback;
		AimCurveCallback.BindDynamic(this, &AGun::Aiming);

		AimTimeline.AddInterpFloat(AimCurve, AimCurveCallback);
		AimTimeline.SetTimelineLength(0.25f);
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// StartAim();

	if (CurrentFireCool > 0.f)
	{
		CurrentFireCool -= DeltaTime;	
	}

	if (bIsAttacking)
	{
		if (CurrentFireCool <= 0.f)
		{
			CurrentFireCool = FireRate;
			FireBullet(WeaponComponent->GetTargetLocation());
			if (FireMode == EFireMode::Single)
			{
				bIsAttacking = false;
			}
		}
	}

	// DrawDebugLine(GetWorld(), GunMesh->GetSocketLocation("Muzzle"), WeaponComponent->GetTargetLocation(), FColor::Red);
}

void AGun::StartFire()
{
	if (true == bIsAttacking) return;
	
	bIsAttacking = true;
}

void AGun::StopFire()
{
	if (false == bIsAttacking) return;
	
	bIsAttacking = false;
}

void AGun::FireBullet(FVector TargetLocation)
{
	ServerRPC_FireBullet(GunMesh->GetSocketTransform("Muzzle"), TargetLocation);
}

void AGun::ToggleFireMode()
{
	if (FireMode == EFireMode::Single) FireMode = EFireMode::Auto;
	else FireMode = EFireMode::Single;

	UE_LOG(LogTemp, Warning, TEXT("AGun::ToggleFireMode) %s"), *UEnum::GetValueAsString(FireMode));
}

void AGun::Reload()
{
}

void AGun::AttachToCharacter()
{
	AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
}

void AGun::DetachFromCharacter()
{
	bIsAiming = false;
}

void AGun::StartAim()
{
	bIsAiming = true;

	// UE_LOG(LogTemp, Warning, TEXT("AGun::StartAim"));
	
	AimStartAction();
}

void AGun::StopAim()
{
	bIsAiming = false;

	// UE_LOG(LogTemp, Warning, TEXT("AGun::StopAim"));
	
	AimStopAction();
}

void AGun::MultiRPC_FailToFire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("AGun::MultiRPC_FailToFire")); 

	if (OwningCharacter->IsLocallyControlled())
	{
		StopFire();
	}
}

void AGun::ControllerRecoil(float Value)
{
	// UE_LOG(LogTemp, Warning, TEXT(" AGun::ControllerRecoil"));
	
	APlayerController* PC = OwningCharacter->GetController<APlayerController>();
	if (PC == nullptr) return;
	
	float LerpValue = FMath::Lerp(0.f, Value, RecoilAmount);
	PC->AddPitchInput(LerpValue * (bIsAiming ? RecoilPitchSpreadWhenAiming : RecoilPitchSpreadWhenNotAiming));

	float YawRecoil = LerpValue / (bIsAiming ? RecoilYawSpreadWhenAiming : RecoilYawSpreadWhenNotAiming);

	if (FMath::RandRange(0.f, 1.f) <= 0.7f)
	{
		YawRecoil *= -1.f;
	}

	PC->AddYawInput(YawRecoil);
}

void AGun::Aiming(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("AGun::Aim) Value : %f"), Value);
	OwningCharacter->GetComponentByClass<USpringArmComponent>()->SetRelativeLocation(FMath::Lerp(FVector::ZeroVector, GunMesh->GetSocketLocation(FName("Sight")), Value));
}

void AGun::AimStartAction_Implementation()
{
}

void AGun::AimStopAction_Implementation()
{
}

void AGun::OnRep_Owner()
{
	Super::OnRep_Owner();

	OwningCharacter = GetOwner<ACharacter>();
	WeaponComponent = OwningCharacter->GetComponentByClass<UWeaponComponent>();
}

void AGun::MultiRPC_FireBullet_Implementation(FTransform MuzzleTransform, FVector TargetLocation)
{
	// if (OwningCharacter->IsLocallyControlled()) return;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleTransform.GetLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFire, MuzzleTransform);

	if (OwningCharacter->IsLocallyControlled())
	{
		ControllerRecoilTimeline.PlayFromStart();
	}
}

void AGun::ServerRPC_FireBullet_Implementation(FTransform MuzzleTransform, FVector TargetLocation)
{
	if (AmmoCount > 0)
	{
		// AmmoCount--;

		if (BulletClass)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AProjectileBase* Bullet =  GetWorld()->SpawnActor<AProjectileBase>(BulletClass, MuzzleTransform.GetLocation(), UKismetMathLibrary::FindLookAtRotation(MuzzleTransform.GetLocation(), TargetLocation), Params);
			Bullet->Init(GetOwner());
		}
		
		MultiRPC_FireBullet(MuzzleTransform, TargetLocation);
	}
	else
	{
		MultiRPC_FailToFire();
	}
}