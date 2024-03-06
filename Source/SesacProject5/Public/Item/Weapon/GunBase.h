// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FPSAnim_Firearm.h"
#include "Interface/WeaponInterface.h"
#include "GunBase.generated.h"

class UWeaponComponent;
class UArrowComponent;
class AProjectileBase;
class USkeletalMeshComponent;
UCLASS()
class SESACPROJECT5_API AGunBase : public AFPSAnim_Firearm, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void FireBullet(FVector TargetLocation) override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireBullet(FVector TargetLocation, FVector FromLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireBullet(FVector FromLocation);

	UFUNCTION(Server, Reliable)
	void ServerRPC_StartFire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopFire();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StartFire();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StopFire();

	virtual void OnRep_Owner() override;
	
	virtual void AttachToCharacter() override;
	virtual void DetachFromCharacter() override;

	// Rload
	virtual void Reload() override;

	void OnReload();
	UFUNCTION(BlueprintCallable)
	void ReloadComplete();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Reload();

	// Aim
	virtual void StartAim() override;
	virtual void StopAim() override;

	// Recoil Function
	virtual float GetRecoilPitch() const override;
	virtual float GetRecoilYaw() const override;

	void AddRecoil();
	virtual void ToggleFireMode() override;
	virtual bool IsAttacking() const override;

private:
	// Recoil Property
	float RecoilPitch = 0.f;
	float RecoilPitchMax = -10.f;
	float RecoilYaw = 0.f;
	
	UPROPERTY()
	UWeaponComponent* WeaponComponent;
	UPROPERTY()
	ACharacter* OwningCharacter;
	UPROPERTY()
	bool bIsTriggered = false;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float FireCoolDown = 0.05f;
	float CurrentFireCoolDown;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<AProjectileBase> BulletClass;

	// Montage
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* ReloadMontage_Character;

	// Sound
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (AllowPrivateAccess))
	USoundBase* FireSound;
};