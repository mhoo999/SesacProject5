// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectiveComponent.generated.h"


class UHealthComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UObjectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectInfo", meta=(AllowPrivateAccess))
	FString objectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectInfo", meta=(AllowPrivateAccess))
	int32 value;

	UHealthComponent* healthComp;

	void OnReturnObjectID(bool bNewIsDead);

public:
	UFUNCTION(Blueprintable)
	FString GetObjectID();

	UFUNCTION(Blueprintable)
	int32 GetValue();

	UFUNCTION(Blueprintable)
	void SetObjectID(FString ID);
	
	UFUNCTION(Blueprintable)
	void SetValue(int32 integer);

	void SetObserveHealth();
};
