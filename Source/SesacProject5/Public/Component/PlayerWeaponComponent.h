// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Component/WeaponComponent.h"
#include "PlayerWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UPlayerWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	virtual FVector GetFocusLocation() const override;

	virtual float GetRecoilPitch() const override;
	virtual float GetRecoilYaw() const override;
private:
	int32 ZeroPoint = 25;
};