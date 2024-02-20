// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM()
enum class EBodyParts : uint8
{
	NONE,
	HEAD,
	THORAX,
	STOMACH,
	LEFTARM,
	RIGHTARM,
	LEFTLEG,
	RIGHTLEG,
	SIZE
};

USTRUCT(BlueprintType)
struct FHealth
{
	GENERATED_BODY()

	void UpdateHealth()
	{
		OnHealthChanged.ExecuteIfBound(Health, MaxHealth);
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
	DECLARE_DELEGATE_TwoParams(FDele_Single_Two_Float_Float, float, float);
	FDele_Single_Two_Float_Float OnHealthChanged;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyDamage(AActor* DamageActor, FName BoneName);

	void ReduceHealth(uint8 BodyPartsIndex, float Damage);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ApplyDamage(EBodyParts BodyParts, float Damage); 

	FHealth& GetHealth(EBodyParts BodyParts);

	UFUNCTION()
	void OnRep_IsDead();
	
private:
	UPROPERTY()
	ACharacter* OwningCharacter;
	UPROPERTY(ReplicatedUsing = "OnRep_IsDead", VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsDead = false;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<FHealth> HealthArray;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TMap<FName, EBodyParts> BodyPartsMap;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TMap<EBodyParts, float> BlackOutDamageModifireMap;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Die, bool);
	FDele_Die OnIsDeadChanged;
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Attacked, AActor*);
	FDele_Attacked OnAttacked;
};