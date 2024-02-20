// copyright by LootingEmployment


#include "Component/HealthComponent.h"

#include "GameFramework/Character.h"
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

	OwningCharacter = GetOwner<ACharacter>();
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

void UHealthComponent::ApplyDamage(AActor* DamageActor, FName BoneName)
{
	if (bIsDead || BodyPartsMap.Contains(BoneName) == false) return;

	IDamageInterface* DamageInterface =  Cast<IDamageInterface>(DamageActor);
	if (DamageInterface == nullptr) return;
	
	OnAttacked.Broadcast(DamageInterface->GetIndicator());
	
	// Todo : At Inventory or Equipment Component, Find Equipment for Block or Ricochet Chance or Reduce Damage

	EBodyParts HittedBodyParts = BodyPartsMap[BoneName];

	UE_LOG(LogTemp, Warning, TEXT("UHealthComponent::ApplyDamage) %s"), *UEnum::GetValueAsString(HittedBodyParts));
	
	ClientRPC_ApplyDamage(HittedBodyParts, DamageInterface->GetDamage());
	if (OwningCharacter->IsLocallyControlled() == false)
	{
		ClientRPC_ApplyDamage_Implementation(HittedBodyParts, DamageInterface->GetDamage());
	}
	
	// // UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %d"), *BoneName.ToString(), BodyPartsIndex);
	// if (HealthArray[BodyPartsIndex].Health == 0.f)
	// {
	// 	// Damage to Destroyed Parts
	// 	float DamageModifier = BlackOutDamageModifireMap[HittedBodyParts];
	//
	// 	for (int i = 1; i < (uint8)EBodyParts::SIZE; ++i)
	// 	{
	// 		if (i == BodyPartsIndex) continue;
	//
	// 		Body
	// 	}
	// }
	// else
	// {
	// 	HealthArray[BodyPartsIndex].Health -= 10.f;
	// 	if (HealthArray[BodyPartsIndex].Health <= 0.f)
	// 	{
	// 		UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Is Dead!"));
	// 		// Destroy Parts
	// 		if (BodyPartsMap[BoneName] == EBodyParts::HEAD || BodyPartsMap[BoneName] == EBodyParts::THORAX)
	// 		{
	// 			bIsDead = true;
	// 			OnRep_IsDead();
	// 		}
	// 	}
	// 	ClientRPC_ApplyDamage(BodyPartsIndex, 10.f);
	// }
}

void UHealthComponent::ReduceHealth(uint8 BodyPartsIndex, float Damage)
{
}

FHealth& UHealthComponent::GetHealth(EBodyParts BodyParts)
{
	return HealthArray[(uint8)BodyParts];
}

void UHealthComponent::OnRep_IsDead()
{
	OnIsDeadChanged.Broadcast(bIsDead);
}

void UHealthComponent::ClientRPC_ApplyDamage_Implementation(EBodyParts BodyParts, float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %f"), *UEnum::GetValueAsString((EBodyParts)BodyParts), Damage);

	uint8 BodyPartsIndex = (uint8)BodyParts;
	
	
	if (HealthArray[BodyPartsIndex].Health <= 0.f)
	{
		if (BodyParts == EBodyParts::HEAD || BodyParts == EBodyParts::THORAX)
		{
			if (OwningCharacter->HasAuthority())
			{
				bIsDead = true;
				OnRep_IsDead();
			}
		}
	}
	else
	{
		HealthArray[BodyPartsIndex].Health -= Damage;

		if (HealthArray[BodyPartsIndex].Health <= 0.f)
		{
			HealthArray[BodyPartsIndex].Health = 0.f;
			if ((BodyParts == EBodyParts::HEAD || BodyParts == EBodyParts::THORAX) && OwningCharacter->HasAuthority())
			{
				bIsDead = true;
				OnRep_IsDead();
			}
		}
	}
	
	HealthArray[BodyPartsIndex].UpdateHealth();
}