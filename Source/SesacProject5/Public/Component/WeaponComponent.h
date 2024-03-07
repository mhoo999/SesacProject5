// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class UMoveComponent;
struct FInputActionValue;
class UInputAction;
class IWeaponInterface;
class ACharacterBase;
class UFPSAnim_CharacterComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

	// Called every frame
    	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void FireBullet();

	void StartFireAction(const FInputActionValue& Value);
	void EndFireAction(const FInputActionValue& Value);

	void ReloadAction(const FInputActionValue& Value);

	void AimStartAction(const FInputActionValue& Value);
	void AimEndAction(const FInputActionValue& Value);

	void ToggleFireModeAction(const FInputActionValue& Value);

	float GetWeaponAttackRange() const;

	void SetSpreadMultiflier(float NewSpreadMultiflier);
	float GetSpreadMultiflier() const;

	UFUNCTION()
	void OnRep_Weapon();

	virtual FVector GetFocusLocation() const;
	virtual void AddRecoil(float Pitch = 0.f, float Yaw = 0.f);
	
	virtual float GetRecoilPitch() const { return 0.f; }
	virtual float GetRecoilYaw() const { return 0.f; }

	virtual FVector GetTargetLocation() const;

	// Reload
	void OnReloadComplete();
	void AddAmmo(int32 AmmoCount);
	void AddAmmoToWeapon(int32 AmmoCount);
	UFUNCTION(Server, Reliable)
	void ServerRPC_ReloadAmmo();
	UFUNCTION()
	void OnRep_RemainAmmo();
	

	UFUNCTION(Server, Reliable)
	void ServerRPC_MakeNoise(); 

	void DestroyWeapon();
	
	UFUNCTION()
	void OnRep_LeftHandIK();

	UFUNCTION()
	void LeftHandIKTimerFunction();

private:
	FTimerHandle LeftHandIKTimerHandle;

protected:
	UPROPERTY()
	float SpreadMultiflier = 1.f;
	
	UPROPERTY()
	UMoveComponent* MoveComponent;
	UPROPERTY()
	ACharacterBase* OwningCharacter;
	UPROPERTY(ReplicatedUsing = "OnRep_Weapon", VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* Weapon;
	IWeaponInterface* WeaponInterface;
private:
	UPROPERTY()
	bool bIsAiming;
	UPROPERTY(ReplicatedUsing = "OnRep_RemainAmmo", Meta = (AllowPrivateAccess))
	int32 RemainAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Fire;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Reload;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Aim;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_ToggleFireMode;

	// Todo : Delete this
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<class AGun> GunClass;

	UPROPERTY(ReplicatedUsing = OnRep_LeftHandIK, Meta = (AllowPrivateAccess))
	FTransform LeftHandIK;
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_IsAiming, bool);
	FDele_IsAiming OnIsAimingChanged;
	DECLARE_DELEGATE_OneParam(FDele_LeftHandIK, FTransform);
	FDele_LeftHandIK OnLeftHandIKChanged;
};