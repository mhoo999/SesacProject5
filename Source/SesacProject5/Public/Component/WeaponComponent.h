// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


struct FInputActionValue;
class UInputAction;
class IWeaponInterface;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFireAction(const FInputActionValue& Value);
	void EndFireAction(const FInputActionValue& Value);

private:
	UPROPERTY(Replicated, VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* Weapon;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Fire;

	// Todo : Delete this
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<class AGunBase> GunClass;
};