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
	}
}

void UWeaponComponent::EndFireAction(const FInputActionValue& Value)
{
	if (WeaponInterface) WeaponInterface->StopFire();
}

void UWeaponComponent::ReloadAction(const FInputActionValue& Value)
{
	if (WeaponInterface) WeaponInterface->Reload();
}

void UWeaponComponent::AimStartAction(const FInputActionValue& Value)
{
	// if (WeaponInterface) WeaponInterface->StartAim();
	if (true == bIsAiming) return;
	bIsAiming = true;
	if (WeaponInterface) WeaponInterface->StartAim();
}

void UWeaponComponent::AimEndAction(const FInputActionValue& Value)
{
	// if (WeaponInterface) WeaponInterface->StopAim();
	if (false == bIsAiming) return;
	bIsAiming = false;
	if (WeaponInterface) WeaponInterface->StopAim();
}

void UWeaponComponent::ToggleFireModeAction(const FInputActionValue& Value)
{
	if (WeaponInterface) WeaponInterface->ToggleFireMode();
}

float UWeaponComponent::GetWeaponAttackRange() const
{
	return 1000.f;
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