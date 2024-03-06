// copyright by LootingEmployment


#include "Component/PlayerWeaponComponent.h"

#include "Character/CharacterBase.h"
#include "Interface/WeaponInterface.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("UPlayerWeaponComponent::TickComponent"));
	
	if (WeaponInterface && WeaponInterface->IsAttacking())
	{
		SpreadMultiflier = FMath::Clamp(SpreadMultiflier - (DeltaTime * AccuracySpeed), 0.3f, 1.0f) ;
		// UE_LOG(LogTemp, Warning, TEXT("UPlayerWeaponComponent::TickComponent) SpreadMultiflier : %f"), SpreadMultiflier);
	}
	else
	{
		SpreadMultiflier = 1.f;
	}
}

FVector UPlayerWeaponComponent::GetFocusLocation() const
{
	FVector CameraDirection = OwningCharacter->GetControlRotation().RotateVector(FVector(1, 0, 0));
	
	return OwningCharacter->GetCameraLocation() + ((ZeroPoint * 100) * CameraDirection); 
}

float UPlayerWeaponComponent::GetRecoilPitch() const
{
	return WeaponInterface->GetRecoilPitch();
}

float UPlayerWeaponComponent::GetRecoilYaw() const
{
	return WeaponInterface->GetRecoilYaw();
}