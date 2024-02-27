// copyright by LootingEmployment


#include "AIComponent/AIWeaponComponent.h"

void UAIWeaponComponent::SetFocusLocation(FVector NewFocusLocation)
{
	FocusLocation = NewFocusLocation;
}

FVector UAIWeaponComponent::GetFocusLocation() const
{
	return FocusLocation;
}

void UAIWeaponComponent::AddRecoil(float Pitch, float Yaw)
{
	// Todo : Add Recoil to AI;
}