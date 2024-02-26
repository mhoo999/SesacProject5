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
	SetIsReplicated(true);

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

	EBodyParts HittedBodyParts = BodyPartsMap[BoneName];
	UE_LOG(LogTemp, Warning, TEXT("UHealthComponent::ApplyDamage) %s"), *UEnum::GetValueAsString(HittedBodyParts));

	IDamageInterface* DamageInterface =  Cast<IDamageInterface>(DamageActor);
	if (DamageInterface == nullptr) return;
	
	OnAttacked.Broadcast(DamageInterface->GetIndicator());

	EDamageType DamageType = DamageInterface->GetDamageType();
	float Damage = DamageInterface->GetDamage();
	if (DamageType == EDamageType::Bullet)
	{
		// Todo : At Inventory or Equipment Component, Find Equipment for Block or Ricochet Chance or Reduce Damage
	}
	
	ClientRPC_ApplyDamage(HittedBodyParts, DamageType, Damage);
	if (OwningCharacter->IsLocallyControlled() == false)
	{
		ClientRPC_ApplyDamage_Implementation(HittedBodyParts, DamageType, Damage);
	}
}

FHealth& UHealthComponent::GetHealth(EBodyParts BodyParts)
{
	return HealthArray[(uint8)BodyParts];
}

void UHealthComponent::OnRep_IsDead()
{
	OnIsDeadChanged.Broadcast(bIsDead);
}

void UHealthComponent::Die()
{
	if (OwningCharacter->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("UHealthComponent::Die"));
		bIsDead = true;
		OnRep_IsDead();
	}
}

void UHealthComponent::ClientRPC_ApplyDamage_Implementation(EBodyParts BodyParts, EDamageType DamageType, float Damage)
{
	// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %f"), *UEnum::GetValueAsString((EBodyParts)BodyParts), Damage);

	uint8 BodyPartsIndex = (uint8)BodyParts;
	
	if (HealthArray[BodyPartsIndex].Health <= 0.f)
	{
		if (DamageType == EDamageType::Bullet)
		{
			if (BodyParts == EBodyParts::HEAD || BodyParts == EBodyParts::THORAX)
			{
				Die();
			}
			else
			{
				// BlackOut Spread Damage

				float BlackOutDamageModifire = BlackOutDamageModifireMap[BodyParts];
				for (int32 i = 1; i < (uint8)EBodyParts::SIZE; ++i)
				{
					if (i == BodyPartsIndex) continue;
					HealthArray[i].SubHealth(Damage * BlackOutDamageModifire);
					if (HealthArray[i].Health == 0.f &&((EBodyParts)i == EBodyParts::HEAD || (EBodyParts)i == EBodyParts::THORAX))
					{
						Die();
					}
				}
			}
		}
	}
	else
	{
		HealthArray[BodyPartsIndex].SubHealth(Damage);
		if (HealthArray[BodyPartsIndex].Health == 0.f)
		{
			UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s is black out"), *UEnum::GetValueAsString((EBodyParts)BodyParts));
			if (BodyParts == EBodyParts::HEAD || BodyParts == EBodyParts::THORAX)
			{
				Die();
			}	
		}
		
	}
}
