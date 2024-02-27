// copyright by LootingEmployment


#include "Component/PlayerWeaponComponent.h"

#include "Character/CharacterBase.h"
#include "Interface/WeaponInterface.h"

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