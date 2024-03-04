// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemData()
		: Name(FText()), Class(nullptr), /*Type(EItemType::NONE), */Image(nullptr)
	{
	}
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> Class;
	// UPROPERTY(EditAnywhere)
	// EItemType Type;
	UPROPERTY(EditAnywhere)
	UTexture2D* Image;
	UPROPERTY(EditAnywhere)
	FIntPoint Size;
	UPROPERTY(EditAnywhere)
	uint32 MaxStack;
	UPROPERTY(EditAnywhere)
	TArray<FText> InteractionNameArray;
};

USTRUCT(BlueprintType)
struct FItemInstance : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemInstance() : Guid(FGuid::NewGuid()), ItemRow(FName()), bIsInStorage(false), bIsRotated(false), CurrentStack(1), bIsFromRaid(false)
	{}

	UPROPERTY(EditAnywhere)
	FGuid Guid;
	UPROPERTY(EditAnywhere)
	FName ItemRow;
	UPROPERTY(EditAnywhere)
	TArray<FGuid> ChildArray;
	UPROPERTY(EditAnywhere)
	bool bIsInStorage;
	UPROPERTY(EditAnywhere)
	bool bIsRotated;
	UPROPERTY(EditAnywhere)
	FIntPoint StoragePosition;
	UPROPERTY(EditAnywhere)
	int CurrentStack;
	UPROPERTY(EditAnywhere)
	bool bIsFromRaid;
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

	void PutToInventory(FStorage* Storage, FIntPoint StoragePosition);
	void PopFromInventory();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PutToInventory();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PopFromInventory();
	
	FItemData GetItemData() const;
	FItemInstance GetItemInstance() const;

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