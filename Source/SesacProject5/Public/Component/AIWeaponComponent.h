// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Component/WeaponComponent.h"
#include "AIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UAIWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	void SetFocusLocation(FVector NewFocusLocation);
	virtual FVector GetFocusLocation() const override;
	virtual void AddRecoil(float Pitch = 0.f, float Yaw = 0.f) override;
private:
	UPROPERTY()
	FVector FocusLocation;
	
};