// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EscapeComponent.generated.h"


class AExitBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UEscapeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEscapeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartEscape(AExitBase* CurrentExit);
	void EndEscape(AExitBase* CurrentExit);

	void SetExitArray(TArray<AExitBase*> NewExitArray);

	UFUNCTION(BlueprintCallable)
	const TArray<AExitBase*>& GetExitArray() const;

	UFUNCTION(Client, Reliable)
	void ClientRPC_Escape();

	UFUNCTION()
	void OnRep_IsEscaping();
	UFUNCTION()
	void OnRep_EscapeTime();
private:
	UPROPERTY(ReplicatedUsing = "OnRep_IsEscaping", Meta = (AllowPrivateAccess))
	bool bIsEscaping = false;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MaxEscapeTime = 5.f; 
	UPROPERTY(ReplicatedUsing = "OnRep_EscapeTime", Meta = (AllowPrivateAccess))
	float EscapeTime = 0.f;

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<AExitBase*> ExitArray;
	
public:
	// Delegate
	
	// bIsEscaping
	DECLARE_DELEGATE_OneParam(FDele_IsEscaping, bool);
	FDele_IsEscaping OnIsEscapingChanged;
	// EscapeTime
	DECLARE_DELEGATE_OneParam(FDele_EscapeTime, float);
	FDele_EscapeTime OnEscapeTimeChanged;

	DECLARE_MULTICAST_DELEGATE(FDele_Escape);
	FDele_Escape OnEscape;
};