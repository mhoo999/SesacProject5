// copyright by LootingEmployment


#include "Component/WeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "Interface/WeaponInterface.h"

#include "Character/CharacterBase.h"
// Todo : Delete this after test
#include "Camera/CameraComponent.h"
#include "Component/MoveComponent.h"
#include "Item/Weapon/Gun.h"
#include "Item/Weapon/GunBase.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseEvent.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, Weapon);
	DOREPLIFETIME(UWeaponComponent, LeftHandIK);
	DOREPLIFETIME(UWeaponComponent, RemainAmmo);
}

void UWeaponComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &UWeaponComponent::StartFireAction);
	PlayerInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &UWeaponComponent::EndFireAction);
	PlayerInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &UWeaponComponent::ReloadAction);
	PlayerInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &UWeaponComponent::AimStartAction);
	PlayerInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &UWeaponComponent::AimEndAction);
	PlayerInputComponent->BindAction(IA_ToggleFireMode, ETriggerEvent::Started, this, &UWeaponComponent::ToggleFireModeAction);
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningCharacter = GetOwner<ACharacterBase>();
	MoveComponent = OwningCharacter->GetComponentByClass<UMoveComponent>();
	// Todo : Delete
	// Debug Test Spawn Gun;
	SetIsReplicated(true);
	if (OwningCharacter->HasAuthority())
	{
		if (GunClass)
		{
			AGun* Gun = GetWorld()->SpawnActor<AGun>(GunClass);
			Weapon = Gun;
			Gun->SetOwner(GetOwner());
			Gun->OnRep_Owner();
			WeaponInterface = Gun;
			WeaponInterface->AttachToCharacter();

			// Set LeftHandTransform
			FTransform LeftHandTransform = WeaponInterface->GetLeftHandTransform();
			FVector OutLocation;
			FRotator OutRotation;
			OwningCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), LeftHandTransform.Rotator(), OutLocation, OutRotation);

			LeftHandIK = FTransform(OutRotation.Quaternion(), OutLocation);

			if (IsRunningDedicatedServer()) return;

			OnRep_LeftHandIK();
		}
	}
}

void UWeaponComponent::FireBullet() 
{
	if (OwningCharacter->IsLocallyControlled())
	{
		WeaponInterface->FireBullet(GetFocusLocation());
	}
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::StartFireAction(const FInputActionValue& Value)
{
	if (WeaponInterface)
	{
		MoveComponent->StopSprint();
		WeaponInterface->StartFire();

		
		ServerRPC_MakeNoise();
	}
}

void UWeaponComponent::EndFireAction(const FInputActionValue& Value)
{
	if (WeaponInterface) WeaponInterface->StopFire();
}

void UWeaponComponent::ReloadAction(const FInputActionValue& Value)
{
	ServerRPC_ReloadAction();
}

void UWeaponComponent::AimStartAction(const FInputActionValue& Value)
{
	// if (WeaponInterface) WeaponInterface->StartAim();
	if (true == bIsAiming) return;
	bIsAiming = true;
	if (WeaponInterface) WeaponInterface->StartAim();
	OnIsAimingChanged.Broadcast(bIsAiming);
}

void UWeaponComponent::AimEndAction(const FInputActionValue& Value)
{
	// if (WeaponInterface) WeaponInterface->StopAim();
	if (false == bIsAiming) return;
	bIsAiming = false;
	if (WeaponInterface) WeaponInterface->StopAim();
	OnIsAimingChanged.Broadcast(bIsAiming);
}

void UWeaponComponent::ToggleFireModeAction(const FInputActionValue& Value)
{
	if (WeaponInterface) WeaponInterface->ToggleFireMode();
}

float UWeaponComponent::GetWeaponAttackRange() const
{
	if (WeaponInterface) return WeaponInterface->GetAttackRange();
	return 100.f;
}

void UWeaponComponent::SetSpreadMultiflier(float NewSpreadMultiflier)
{
	SpreadMultiflier = NewSpreadMultiflier;
}

float UWeaponComponent::GetSpreadMultiflier() const
{
	return SpreadMultiflier;
}

void UWeaponComponent::OnRep_Weapon()
{
	WeaponInterface = Cast<IWeaponInterface>(Weapon);
}

FVector UWeaponComponent::GetFocusLocation() const
{
	return FVector();
}

void UWeaponComponent::AddRecoil(float Pitch, float Yaw)
{
}

FVector UWeaponComponent::GetTargetLocation() const
{
	return OwningCharacter->GetCameraLocation() + (OwningCharacter->GetComponentByClass<UCameraComponent>()->GetForwardVector() * 2500.f);
}

void UWeaponComponent::OnReloadComplete()
{
	if (OwningCharacter->IsLocallyControlled() == false) return;

	ServerRPC_ReloadAmmo();
}

void UWeaponComponent::AddAmmo(int32 AmmoCount)
{
	RemainAmmo += AmmoCount;
}

void UWeaponComponent::AddAmmoToWeapon(int32 AmmoCount)
{
	WeaponInterface->AddAmmo(AmmoCount);
}

void UWeaponComponent::ServerRPC_ReloadAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("UWeaponComponent::ReloadAction) RemainAmmo : %d"), RemainAmmo);
	if (WeaponInterface && RemainAmmo > 0)
	{
		WeaponInterface->Reload();
	}
}

void UWeaponComponent::ServerRPC_ReloadAmmo_Implementation()
{
	int32 AddCount = RemainAmmo > 30 ? 30 : RemainAmmo;
	AddAmmoToWeapon(AddCount);
	AddAmmo(-AddCount);
}

void UWeaponComponent::OnRep_RemainAmmo()
{
}

void UWeaponComponent::DestroyWeapon()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}
}

void UWeaponComponent::OnRep_LeftHandIK()
{
	if (OnLeftHandIKChanged.IsBound() == false)
	{
		if (LeftHandIKTimerHandle.IsValid()) return;
		
		GetWorld()->GetTimerManager().SetTimer(LeftHandIKTimerHandle, this, &UWeaponComponent::LeftHandIKTimerFunction, 3.0f, true);
		
		// UE_LOG(LogTemp, Warning, TEXT("UWeaponComponent::OnRep_LeftHandIK) Is not bounded"));
	}
	OnLeftHandIKChanged.ExecuteIfBound(LeftHandIK);
}

void UWeaponComponent::LeftHandIKTimerFunction()
{
	if (OnLeftHandIKChanged.IsBound() == false) return;

	GetWorld()->GetTimerManager().ClearTimer(LeftHandIKTimerHandle);
	OnLeftHandIKChanged.ExecuteIfBound(LeftHandIK);
	
	UE_LOG(LogTemp, Warning, TEXT("UWeaponComponent::LeftHandIKTimerFunction) Finally it is bounded"));
}

void UWeaponComponent::ServerRPC_MakeNoise_Implementation()
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), OwningCharacter->GetActorLocation(), 10.f, OwningCharacter, 200.f);
}
