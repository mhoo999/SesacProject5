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
	UPlayerWeaponComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FVector GetFocusLocation() const override;

	virtual float GetRecoilPitch() const override;
	virtual float GetRecoilYaw() const override;
private:
	int32 ZeroPoint = 25;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float AccuracySpeed = 1.f;
};