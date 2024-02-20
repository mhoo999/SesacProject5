// copyright by LootingEmployment


#include "Component/PlayerWeaponComponent.h"

#include "Character/CharacterBase.h"

FVector UPlayerWeaponComponent::GetFocusLocation() const
{
	FVector CameraDirection = OwningCharacter->GetControlRotation().RotateVector(FVector(1, 0, 0));
	
	return OwningCharacter->GetCameraLocation() + ((ZeroPoint * 100) * CameraDirection); 
}

void UPlayerWeaponComponent::AddRecoil()
{
	if (OwningCharacter->IsLocallyControlled())
	{
		OwningCharacter->AddControllerPitchInput(FMath::RandRange(-0.f, -1.f));
		OwningCharacter->AddControllerYawInput(FMath::RandRange(-1.0f, 1.0f));
	}
}