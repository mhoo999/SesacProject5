// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.generated.h"


// class UInventoryComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_EquipmentArray();

	UFUNCTION()
	void OnRep_StorageArray();

	bool PutItem(AItemBase* Item);

	
	UFUNCTION(Client, Reliable)
	void ClientRPC_LootItem(UInventoryComponent* TargetInventory);

	const TArray<FStorage>& GetStorageArray() const;
	int32 GetItemCount(const FString& ItemName) const;

	UFUNCTION(CallInEditor)
	void TestCheckCan();
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentArray, Meta = (AllowPrivateAccess))
	TArray<AActor*> EquipmentArray;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_StorageArray, Meta = (AllowPrivateAccess))
	TArray<FStorage> StorageArray;

public:
	// Delegate
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_StorageArray, const TArray<FStorage>&);
	FDele_StorageArray OnInventoryChanged;
};