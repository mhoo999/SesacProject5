// copyright by LootingEmployment


#include "Component/HealthComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/ProjectileBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	HealthArray.Init(FHealth(), (uint8)EBodyParts::SIZE);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->HasAuthority())
	{
		SetIsReplicated(true);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, bIsDead);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

void UHealthComponent::ApplyDamage(FProjectileInfo ProjectileInfo, FName BoneName)
{
	if (bIsDead || BodyPartsMap.Contains(BoneName) == false) return;
	uint8 BodyPartsIndex = (uint8)BodyPartsMap[BoneName];
	// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %d"), *BoneName.ToString(), BodyPartsIndex);
	if (HealthArray[BodyPartsIndex].Health == 0.f)
	{
		// Damage to Destroyed Parts
	}
	else
	{
		HealthArray[BodyPartsIndex].Health -= 10.f;
		if (HealthArray[BodyPartsIndex].Health <= 0.f)
		{
			// Destroy Parts
			if (BodyPartsMap[BoneName] == EBodyParts::HEAD || BodyPartsMap[BoneName] == EBodyParts::THORAX)
			{
				bIsDead = true;
			}
		}
	}
	ClientRPC_ApplyDamage(BodyPartsIndex, 10.f);
}

FHealth& UHealthComponent::GetHealth(EBodyParts BodyParts)
{
	return HealthArray[(uint8)BodyParts];
}

bool UHealthComponent::IsDead() const
{
	return bIsDead;
}

void UHealthComponent::ClientRPC_ApplyDamage_Implementation(uint8 BodyParts, float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %f"), *UEnum::GetValueAsString((EBodyParts)BodyParts), Damage);
	HealthArray[BodyParts].Health -= Damage;
	HealthArray[BodyParts].UpdateHealth();
}
