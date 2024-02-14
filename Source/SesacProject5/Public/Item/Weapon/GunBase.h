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

	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopFire();

private:
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
};