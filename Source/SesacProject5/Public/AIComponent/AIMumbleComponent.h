// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIMumbleComponent.generated.h"


class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UAIMumbleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIMumbleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ACharacterBase* ai;
	
	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	USoundBase* Attackmumble;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	USoundBase* mumble1;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	USoundBase* mumble2;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	USoundBase* mumble3;

	USoundBase* mumbleSound;
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCPlayAttackMumble();

	UFUNCTION(Server, Reliable)
	void ServerRPCPlayPatrolMumble();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCPlayPatrolMumble(int32 randValue);
};
