// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DamageInterface.h"
#include "ProjectileBase.generated.h"

class ADecalActor;
class UProjectileMovementComponent;
class USphereComponent;

USTRUCT()
struct FProjectileInfo
{
	GENERATED_BODY()
};

UCLASS()
class SESACPROJECT5_API AProjectileBase : public AActor, public IDamageInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	virtual EDamageType GetDamageType() const override;
	virtual float GetDamage() const override;
	virtual AActor* GetIndicator() const override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FProjectileInfo ProjectileInfo;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USphereComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UMaterialInstance* DecalInstance;
};