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
	void OnRepEquipmentArray();

	UFUNCTION()
	void OnRepStorageArray();

	bool PutItem(AItemBase* Item);

	
	UFUNCTION(Client, Reliable)
	void ClientRPC_LootItem(UInventoryComponent* TargetInventory);
private:
	UPROPERTY(ReplicatedUsing = OnRepEquipmentArray, Meta = (AllowPrivateAccess))
	TArray<AActor*> EquipmentArray;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRepStorageArray, Meta = (AllowPrivateAccess))
	TArray<FStorage> StorageArray;

public:
	// Delegate
	DECLARE_MULTICAST_DELEGATE_OneParam(FDele_StorageArray, TArray<FStorage>&);
	FDele_StorageArray OnInventoryChanged;
};