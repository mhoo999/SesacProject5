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

	FItemData()
		: Name("None"), ItemSize(0,0), MaxStack(-1)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> InteractionNameArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;
};

USTRUCT()
struct FItemInstance : public FTableRowBase
{
	GENERATED_BODY()

	FItemInstance()
		: InventoryPos(-1, -1), CurrentStack(-1)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint InventoryPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStack;
};

struct FStorage;
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* InteractCharacter, FText InteractionName) override;
	virtual const TArray<FText>& GetInteractionNameArray() override;
	virtual FText GetActorName() const override;

	void PutToInventory(FStorage* Storage, FIntPoint InventoryPos);
	void PopFromInventory();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PutToInventory(); 
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PopFromInventory(); 

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UBoxComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FItemData ItemData;
	UPROPERTY(Replicated, EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FItemInstance ItemInstance;
};