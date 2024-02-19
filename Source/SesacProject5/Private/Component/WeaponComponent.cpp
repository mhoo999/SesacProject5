// copyright by LootingEmployment


#include "Component/WeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "Interface/WeaponInterface.h"

// Todo : Delete this after test
#include "GameFramework/Character.h"
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
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Todo : Delete
	// Debug Test Spawn Gun;
	if (GetOwner()->HasAuthority())
	{
		SetIsReplicated(true);
		if (GunClass)
		{
			AGunBase* Gun = GetWorld()->SpawnActor<AGunBase>(GunClass);
			Weapon = Gun;
			Gun->AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunSocket"));
			Gun->SetOwner(GetOwner());
			Gun->OnRep_Owner();
			WeaponInterface = Gun;
		}
	}
}

void UWeaponComponent::FireBullet() 
{
	WeaponInterface->FireBullet();
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::StartFireAction(const FInputActionValue& Value)
{
	WeaponInterface->StartFire();
}

void UWeaponComponent::EndFireAction(const FInputActionValue& Value)
{
	WeaponInterface->StopFire();
}

float UWeaponComponent::GetWeaponAttackRange() const
{
	return 1000.f;
}

void UWeaponComponent::OnRep_Weapon()
{
	WeaponInterface = Cast<IWeaponInterface>(Weapon);
}