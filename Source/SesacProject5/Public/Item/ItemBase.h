// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "ItemBase.generated.h"

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> InteractionNameArray;
};

class UBoxComponent;
UCLASS()
class SESACPROJECT5_API AItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

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
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UBoxComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FItemData ItemData;
};