// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "DoorBase.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* InteractCharacter, FText InteractionName) override;
	virtual const TArray<FText>& GetInteractionNameArray() const override;
	virtual FText GetActorName() const override;

private:
	TArray<FText> InteractionNameArray;
};