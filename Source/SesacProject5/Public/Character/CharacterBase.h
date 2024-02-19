// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UEscapeComponent;
class UHealthComponent;
class UWeaponComponent;
class UInteractComponent;
class UInputMappingContext;
class UMoveComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SESACPROJECT5_API ACharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UMoveComponent* MoveComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UInteractComponent* InteractComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UWeaponComponent* WeaponComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UEscapeComponent* EscapeComponent;

	
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultIMC;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	int32 TeamId;
};