// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Openable.generated.h"

class UInventoryComponent;

UENUM()
enum class EOpenableState : uint8
{
	NONE,
	OPEN,
	CLOSE,
	LOCKED,
	UNLOCKED,
	SIZE,
};

UCLASS()
class SESACPROJECT5_API AOpenable : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOpenable();

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

	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_Open(ACharacter* InteractCharacter);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Close();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Unlock();
	
	virtual void Open();
	virtual void Close();
	virtual void Unlock();

	UFUNCTION()
	void OnRep_OpenableState();

protected:
	UPROPERTY()
	TArray<FText> InteractionNameArray;
	UPROPERTY(ReplicatedUsing = "OnRep_OpenableState", EditAnywhere, Meta = (AllowPrivateAccess))
	EOpenableState OpenableState = EOpenableState::CLOSE;
private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FName KeyName;
};