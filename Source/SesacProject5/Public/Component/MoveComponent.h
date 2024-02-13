// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

struct FInputActionValue;
class UInputAction;
class UEnhancedInputComponent;
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESACPROJECT5_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

	bool IsSprint() const;

	void MoveAction(const FInputActionValue& Value);
	void LookAction(const FInputActionValue& Value);
	void CrouchAction(const FInputActionValue& Value);
	void SprintStartAction(const FInputActionValue& Value);
	void SprintEndAction(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetMaxWalkSpeed(float NewMaxWalkSpeed);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetMaxWalkSpeed(float NewMaxWalkSpeed);
	
	
private:
	bool bIsSprint;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Crouch;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInputAction* IA_Sprint;

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	ACharacter* OwningCharacter;
};