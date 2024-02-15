// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/WeaponInterface.h"
#include "GunBase.generated.h"

class UArrowComponent;
class AProjectileBase;
class USkeletalMeshComponent;
UCLASS()
class SESACPROJECT5_API AGunBase : public AActor, public IWeaponInterface
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
	virtual void FireBullet() override;
	UFUNCTION(Server, Reliable)
	void ServerRPC_FireBullet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StartFire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopFire();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StartFire();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StopFire();

	virtual void OnRep_Owner() override;

private:
	UPROPERTY()
	ACharacter* OwningCharacter;
	UPROPERTY()
	bool bIsTriggered = false;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float FireCoolDown = 0.05f;
	float CurrentFireCoolDown;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<AProjectileBase> BulletClass;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UArrowComponent* FireArrowComponent;

	// Montage
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* FireMontage;
};