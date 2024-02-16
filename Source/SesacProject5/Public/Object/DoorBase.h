// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "DoorBase.generated.h"

UENUM()
enum class EDoorState : uint8
{
	NONE,
	OPEN,
	CLOSE,
	LOCKED
};

UCLASS()
class SESACPROJECT5_API ADoorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* InteractCharacter, FText InteractionName) override;
	virtual const TArray<FText>& GetInteractionNameArray() override;
	virtual FText GetActorName() const override;

	void Open();
	void Close();

	// OnRep
	UFUNCTION()
	void OnRep_DoorState();

private:
	UPROPERTY(ReplicatedUsing = "OnRep_DoorState", EditAnywhere, Meta = (AllowPrivateAccess))
	EDoorState DoorState;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FName KeyName;
	
	UPROPERTY()
	TArray<FText> InteractionNameArray;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* DoorFrameMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* DoorMeshComponent;
};