// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class UHealthComponent;
/**
 * 
 */
class UWeaponComponent;
UCLASS()
class SESACPROJECT5_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override; 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_FireBullet();
private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> DeathMontageArray;
	UPROPERTY()
	ACharacter* PlayerCharacter;
	UPROPERTY()
	UWeaponComponent* WeaponComponent;
	UPROPERTY()
	UHealthComponent* HealthComponent;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float PitchAngle;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MoveSpeed;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MoveDirection;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsCrouched = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsDead = false;
};