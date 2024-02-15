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
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, HeadHealth);
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

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ApplyDamage(FProjectileInfo ProjectileInfo, FName BoneName)
{
	if (BoneName.IsEqual("neck_01") || BoneName.IsEqual("head") || BoneName.IsEqual("eyebrows") || BoneName.IsEqual("eyes"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Head"));
	}
	else if (BoneName.IsEqual("Pelvis") || BoneName.IsEqual("spine_01"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Stomach"));
		StomachHealth.Health -= 10.f;
	}
	else if (BoneName.IsEqual("spine_02") || BoneName.IsEqual("spine_03"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Thorax"));
		ThoraxHealth.Health -= 10.f;
	}
	else if (BoneName.IsEqual("clavicle_l") || BoneName.IsEqual("UpperArm_L") || BoneName.IsEqual("lowerarm_l") || BoneName.IsEqual("Hand_L"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Arm_L")); 
	}
	else if (BoneName.IsEqual("clavicle_r") || BoneName.IsEqual("UpperArm_R") || BoneName.IsEqual("lowerarm_r") || BoneName.IsEqual("Hand_R"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Arm_R")); 
	}
	else if (BoneName.IsEqual("Thigh_L") || BoneName.IsEqual("calf_l") || BoneName.IsEqual("Foot_L") || BoneName.IsEqual("ball_l"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Leg_L")); 
	}
	else if (BoneName.IsEqual("Thigh_R") || BoneName.IsEqual("calf_r") || BoneName.IsEqual("Foot_R") || BoneName.IsEqual("ball_r"))
	{
		// UE_LOG(LogTemp, Log, TEXT("UHealthComponent::ApplyDamage) Leg_R")); 
	}
}

void UHealthComponent::OnRep_HeadHealth()
{
	OnHeadHealthChanged.ExecuteIfBound(HeadHealth.Health, HeadHealth.MaxHealth);
}

void UHealthComponent::OnRep_ThoraxHealth()
{
	OnThoraxHealthChanged.ExecuteIfBound(ThoraxHealth.Health, ThoraxHealth.MaxHealth);
}

void UHealthComponent::OnRep_StomachHealth()
{
	OnStomachHealthChanged.ExecuteIfBound(StomachHealth.Health, StomachHealth.MaxHealth);
}