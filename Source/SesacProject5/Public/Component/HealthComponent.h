// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FHealth
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyDamage(struct FProjectileInfo ProjectileInfo, FName BoneName);

	UFUNCTION()
	void OnRep_HeadHealth();
	UFUNCTION()
	void OnRep_ThoraxHealth();
	UFUNCTION()
	void OnRep_StomachHealth();
private:
	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_HeadHealth", Meta = (AllowPrivateAccess))
	FHealth HeadHealth;
	DECLARE_DELEGATE_TwoParams(FDele_Single_Two_Float_Float, float, float);
	FDele_Single_Two_Float_Float OnHeadHealthChanged;

	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_ThoraxHealth", Meta = (AllowPrivateAccess))
	FHealth ThoraxHealth;
	DECLARE_DELEGATE_TwoParams(FDele_Single_Two_Float_Float, float, float);
	FDele_Single_Two_Float_Float OnThoraxHealthChanged;

	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_StomachHealth", Meta = (AllowPrivateAccess))
	FHealth StomachHealth;
	DECLARE_DELEGATE_TwoParams(FDele_Single_Two_Float_Float, float, float);
	FDele_Single_Two_Float_Float OnStomachHealthChanged;
};