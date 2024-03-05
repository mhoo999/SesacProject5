// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Openable.h"
#include "GameFramework/Actor.h"
#include "DoorBase.generated.h"

UCLASS()
class SESACPROJECT5_API ADoorBase : public AOpenable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual const TArray<FText>& GetInteractionNameArray() override;
	virtual FText GetActorName() const override;

	virtual void Open() override;
	virtual void Close() override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* DoorFrameMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* DoorMeshComponent;

	UPROPERTY()
	FRotator DoorMeshTargetRotation;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float RotationMultiplier = 1.5f;

	// Sound
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* OpenSound;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* CloseSound;
};