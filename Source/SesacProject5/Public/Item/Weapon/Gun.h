// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/WeaponInterface.h"
#include "Gun.generated.h"

class UWeaponComponent;
class AProjectileBase;
class UFPSAnimInstance;

UENUM()
enum class EFireMode : uint8
{
	None,
	Single,
	Auto
};

class USkeletalMeshComponent;
UCLASS()
class SESACPROJECT5_API AGun : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void StartFire() override; 
	virtual void StopFire() override;

	virtual void FireBullet(FVector TargetLocation) override;
	virtual void ToggleFireMode() override;
	virtual void Reload() override;
	virtual void AttachToCharacter() override;
	virtual void DetachFromCharacter() override;
	
	virtual void StartAim() override;
	virtual void StopAim() override;

	virtual FTransform GetLeftHandTransform() override;

	// Ammo
	virtual void AddAmmo(int32 AmmoCount) override;

	// FireBullet

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireBullet(FTransform MuzzleTransform, FVector TargetLocation, float SpreadMultiflier);  
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireBullet(FTransform MuzzleTransform, FVector TargetLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FailToFire();

	UFUNCTION()
	void ControllerRecoil(float Value);
	UFUNCTION()
	void Aiming(float Value);

	UFUNCTION(BlueprintNativeEvent)
	void AimStartAction();
	UFUNCTION(BlueprintNativeEvent)
	void AimStopAction();

	// Check Wall
	UFUNCTION()
	void CheckWallFunction();
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetWallValue(float NewWallValue);

	UFUNCTION()
	void OnRep_WallDistance();
	UFUNCTION()
	void OnRep_CurrentAmmoCount();

	virtual void OnRep_Owner() override;
	virtual bool IsAttacking() const override;
	virtual float GetAttackRange() const override;

private:
	// Range
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float AttackRange = 65000.f;
	// Spread
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float StartSpreadAmount = 100.f;

	// Check Wall
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float WeaponLength = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_WallDistance, Meta = (AllowPrivateAccess))
	float WallDistance;
	
	// Recoil
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FTimeline ControllerRecoilTimeline;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UCurveFloat* ControllerRecoilCurve;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RecoilAmount = 0.15f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RecoilPitchSpreadWhenAiming = 0.25f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RecoilPitchSpreadWhenNotAiming = 1.f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RecoilYawSpreadWhenAiming = 2.5f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RecoilYawSpreadWhenNotAiming = 1.f;

	// Aim
	bool bIsAiming = false;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FTimeline AimTimeline;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UCurveFloat* AimCurve;
	
	UPROPERTY()
	ACharacter* OwningCharacter;
	UPROPERTY()
	UWeaponComponent* WeaponComponent;
	UPROPERTY()
	UFPSAnimInstance* AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_CurrentAmmoCount, Meta = (AllowPrivateAccess))
	int32 CurrentAmmoCount = 30;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	EFireMode FireMode = EFireMode::Single;
	UPROPERTY()
	bool bIsAttacking;

	// Cool
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float CurrentFireCool = 0.f;

	// Sound
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	USoundBase* FireSound;

	// Emitter
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	UParticleSystem* MuzzleFire;

	// Component
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess))
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess))
	USceneComponent* AimOffset;

	// Bullet
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TSubclassOf<AProjectileBase> BulletClass;
};