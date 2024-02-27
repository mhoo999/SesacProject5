// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACharacter;
class SESACPROJECT5_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void StartFire() = 0;
	virtual void StopFire() = 0;

	virtual void Reload() = 0;

	virtual void AttachToCharacter() = 0;
	virtual void DetachFromCharacter() = 0;

	virtual void StartAim() = 0;
	virtual void StopAim() = 0;

	virtual void FireBullet(FVector TargetLocation) = 0;

	virtual float GetRecoilPitch() const {return 0;}
	virtual float GetRecoilYaw() const {return 0;}
};