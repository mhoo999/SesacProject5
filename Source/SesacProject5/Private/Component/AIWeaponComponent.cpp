// copyright by LootingEmployment


#include "Component/AIWeaponComponent.h"

void UAIWeaponComponent::SetFocusLocation(FVector NewFocusLocation)
{
	FocusLocation = NewFocusLocation;
}

FVector UAIWeaponComponent::GetFocusLocation() const
{
	return FocusLocation;
}

void UAIWeaponComponent::AddRecoil()
{
	// Todo : Add Recoil to AI;
}