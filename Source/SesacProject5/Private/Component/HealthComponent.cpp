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

	// HealthMap[EBodyParts::HEAD] = HeadHealth;
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
	
	// Todo : At Inventory or Equipment Component, Find Equipment for Block or Ricochet Chance or Reduce Damage
	
	EBodyParts HittedBodyParts = BodyPartsMap[BoneName];

	OnAttacked.Broadcast(Cast<IDamageInterface>(DamageActor)->GetIndicator());
	
	// uint8 BodyPartsIndex = (uint8)HittedBodyParts;
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

void UHealthComponent::ClientRPC_ApplyDamage_Implementation(uint8 BodyParts, float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ClientRPC_ApplyDamage_Implementation) %s, %f"), *UEnum::GetValueAsString((EBodyParts)BodyParts), Damage);

	// Todo : Dedi Server 쓰면 If문 없애고 그냥 안에꺼 실행
	if (GetOwner()->HasAuthority() == false)
	{
		HealthArray[BodyParts].Health -= Damage;
	}
	HealthArray[BodyParts].UpdateHealth();
}
