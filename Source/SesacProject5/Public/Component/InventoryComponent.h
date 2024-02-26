// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemBase.h"
#include "InventoryComponent.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FStorage
{
	GENERATED_BODY()
	
	FStorage()
		: Size(0, 0)
	{
	}

	bool AddItem(AItemBase* Item)
	{
		for (int i = 0; i < Size.Y; ++i)
		{
			for (int j = 0; j < Size.X; ++j)
			{
				if (false == IsOccupied[i * Size.X + j])
				{
					IsOccupied[i * Size.X + j] = true;
					ItemArray.Add(Item);
					Item->PutToInventory(this, FIntPoint(i, j));
					return true;
				}
			}
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> IsOccupied;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AItemBase*> ItemArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Size;
};


class UInventoryWidget;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UInventoryWidget* GetInventoryWidget();
	void HideInventoryWidget();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnRep_StorageArray();

	TArray<FStorage>& GetStorageArray();

	void AddItem(AItemBase* Item);
private:
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_StorageArray, Meta = (AllowPrivateAccess))
	TArray<FStorage> StorageArray;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

public:
	DECLARE_DELEGATE_OneParam(FDele_ItemArray, TArray<FStorage>);
	FDele_ItemArray OnStorageArrayChanged;
};