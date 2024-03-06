// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

enum class EDamageType : uint8;
class IDamageInterface;

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

	void SubHealth(float Damage)
	{
		Health -= Damage;
		if (Health <= 0.f)
		{
			Health = 0.f;
		}
		OnHealthChanged.Broadcast(BodyParts, Health, MaxHealth);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyParts BodyParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_Three_BodyParts_Health_MaxHealth, EBodyParts, float, float);
	FDele_Multi_Three_BodyParts_Health_MaxHealth OnHealthChanged;
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

	UFUNCTION(Client, Reliable)
	void ClientRPC_ApplyDamage(EBodyParts BodyParts, EDamageType DamageType, float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StartDieSound();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StartHurtSound();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_StopHurtSound();

	FHealth& GetHealth(EBodyParts BodyParts);

	bool IsDead() const;

	UFUNCTION()
	void OnRep_IsDead();

	UFUNCTION()
	void Die();

	// Getter
	float GetTotalHealth() const;

	AActor* GetAttackActor() const;
	
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

	UPROPERTY(Replicated)
	AActor* AttackActor;
	
	float HurtHealth = 200.f;
	
	// Sound
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UAudioComponent* VoiceComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* DieSound;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* HurtSound;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Die, bool);
	FDele_Die OnIsDeadChanged;
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Attacked, AActor*);
	FDele_Attacked OnAttacked;
};