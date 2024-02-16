// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

class IInteractInterface;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UEnhancedInputComponent;
class UInteractWidget;
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();
	
	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setter
	void SetInteractWidget(UInteractWidget* NewInteractWidget);

	// RPC
	UFUNCTION(Server, Reliable)
	void ServerRPC_Interact(AActor* InteractTarget, const FText& InteractionName); 

private:
	void InteractAction(const FInputActionValue& Value);
	void SelectInteractionAction(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AActor* InteractActor;

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	ACharacter* OwningCharacter;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float TraceDistance = 1000.f;

	UPROPERTY()
	UInteractWidget* InteractWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_Interact;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* IA_SelectInteraction;

public:
	DECLARE_DELEGATE_OneParam(FDele_Interact, IInteractInterface*);
	FDele_Interact OnInteractActorChanged;
};