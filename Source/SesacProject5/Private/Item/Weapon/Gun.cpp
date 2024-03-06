// copyright by LootingEmployment


#include "Item/Weapon/Gun.h"

#include "Net/UnrealNetwork.h"
#include "AnimInstance/FPSAnimInstance.h"
#include "Camera/CameraComponent.h"
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
			FireBullet(WeaponComponent->GetFocusLocation());
			if (FireMode == EFireMode::Single)
			{
				bIsAttacking = false;
			}
		}
	}

	if (OwningCharacter && OwningCharacter->IsLocallyControlled())
	{
		CheckWallFunction();
	}

	// DrawDebugLine(GetWorld(), GunMesh->GetSocketLocation("Muzzle"), WeaponComponent->GetTargetLocation(), FColor::Red);
}

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun, WallDistance);
	DOREPLIFETIME(AGun, CurrentAmmoCount);
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
	ServerRPC_FireBullet(GunMesh->GetSocketTransform("Muzzle"), TargetLocation, WeaponComponent->GetSpreadMultiflier());
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

FTransform AGun::GetLeftHandTransform()
{
	return GunMesh->GetSocketTransform("LeftHandIK");
}

void AGun::AddAmmo(int32 AmmoCount)
{
	CurrentAmmoCount += AmmoCount;
	if (GetNetMode() != NM_DedicatedServer)
	{
		OnRep_CurrentAmmoCount();
	}
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
	// UE_LOG(LogTemp, Warning, TEXT("AGun::Aim) Value : %f"), Value);
	OwningCharacter->GetComponentByClass<USpringArmComponent>()->SetRelativeLocation(FMath::Lerp(FVector::ZeroVector, GunMesh->GetSocketLocation(FName("Sight")), Value));
}

void AGun::AimStartAction_Implementation()
{
}

void AGun::AimStopAction_Implementation()
{
}

void AGun::CheckWallFunction()
{
	UCameraComponent* CameraComponent = OwningCharacter->GetComponentByClass<UCameraComponent>();
	FVector Start = CameraComponent->GetComponentLocation();
	FVector ForwardVector = CameraComponent->GetForwardVector();
	Start += ForwardVector;
	FVector End = Start + ForwardVector  * WeaponLength;
	FHitResult OutHit;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwningCharacter);
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, Params))
	{
		// UE_LOG(LogTemp, Warning, TEXT("AGun::CheckWallFunction) Hit Actor : %s"), *OutHit.GetActor()->GetActorNameOrLabel());

		ServerRPC_SetWallValue(FVector::Distance(OutHit.Location, Start) / WeaponLength);
	}
	else
	{
		ServerRPC_SetWallValue(1.f);
	}
}

void AGun::ServerRPC_SetWallValue_Implementation(float NewWallValue)
{
	WallDistance = NewWallValue;
	OnRep_WallDistance();
}

void AGun::OnRep_WallDistance()
{
	if (AnimInstance)
	{
		AnimInstance->SetWallTargetValue(WallDistance);
	}
}

void AGun::OnRep_CurrentAmmoCount()
{
	// Connect to widget
}

void AGun::OnRep_Owner() 
{
	Super::OnRep_Owner();

	OwningCharacter = GetOwner<ACharacter>();
	WeaponComponent = OwningCharacter->GetComponentByClass<UWeaponComponent>();
	AnimInstance =  Cast<UFPSAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance());
}

bool AGun::IsAttacking() const
{
	return bIsAttacking;
}

void AGun::MultiRPC_FireBullet_Implementation(FTransform MuzzleTransform, FVector TargetLocation)
{
	// if (OwningCharacter->IsLocallyControlled()) return;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleTransform.GetLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFire, MuzzleTransform);

	AnimInstance->ProcedualRecoil(1.5f);

	if (OwningCharacter->IsLocallyControlled())
	{
		ControllerRecoilTimeline.PlayFromStart();
	}
}

void AGun::ServerRPC_FireBullet_Implementation(FTransform MuzzleTransform, FVector TargetLocation, float SpreadMultiflier)
{
	if (CurrentAmmoCount > 0)
	{
		CurrentAmmoCount--;
		
		// Right Vector
		FVector RightSpreadVector = FRotationMatrix(MuzzleTransform.Rotator()).GetScaledAxis(EAxis::Y) * FMath::RandRange(-StartSpreadAmount, StartSpreadAmount) * SpreadMultiflier;
		// Up Vector
		FVector UpSpreadVector = FRotationMatrix(MuzzleTransform.Rotator()).GetScaledAxis(EAxis::Z) * FMath::RandRange(-StartSpreadAmount, StartSpreadAmount) * SpreadMultiflier;

		TargetLocation += (RightSpreadVector + UpSpreadVector);
		
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