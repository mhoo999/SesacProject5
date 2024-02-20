// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


struct FInputActionValue;
class UInputAction;
class IWeaponInterface;
class ACharacterBase;

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

	float GetWeaponAttackRange() const;

	UFUNCTION()
	void OnRep_Weapon();

	virtual FVector GetFocusLocation() const;
	virtual void AddRecoil();

protected:
	UPROPERTY()
	ACharacterBase* OwningCharacter;
private:
	UPROPERTY(ReplicatedUsing = "OnRep_Weapon", VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* Weapon;
	IWeaponInterface* WeaponInterface;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Fire;

	// Todo : Delete this
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<class AGunBase> GunClass;
};