// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Openable.h"
#include "GameFramework/Actor.h"
#include "LootBox.generated.h"

class AItemBase;
class UInventoryComponent;

UCLASS()
class SESACPROJECT5_API ALootBox : public AOpenable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual FText GetActorName() const override;

	virtual void ServerRPC_Open(ACharacter* InteractCharacter) override;

	virtual void Open() override;
	virtual void Close() override;

	UFUNCTION(CallInEditor)
	void InitBox();
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TArray<TSubclassOf<AItemBase>> ItemClassArray;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* BoxMeshComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* LidMeshComponent;
};