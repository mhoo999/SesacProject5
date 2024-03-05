// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UFUNCTION()
	void Die(bool bNewIsDead);

	UFUNCTION()
	void AnimNotify_OnDieEnd();

	UFUNCTION()
	void UpdateIsSprint(bool bNewIsSprint);
	UFUNCTION()
	void UpdateIsAiming(bool bNewIsAiming);
	UFUNCTION()
	void UpdateHandSwayFloats(float NewSidMove, float NewMouseX, float NewMouseY);
	UFUNCTION()
	void UpdateLeftHandIK(FTransform NewLeftHandIK);

	UFUNCTION(BlueprintNativeEvent)
	void ProcedualRecoil(float Multiplier);

	void LeanInterpolaction(float TargetFloat, float DeltaSeconds);

private:
	UPROPERTY()
	bool bIsDead = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsCrouch = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsSprint = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsAiming = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float SideMove;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MouseX;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MouseY;


	// Lean Function
	UPROPERTY()
	bool bLeanLeft;
	UPROPERTY()
	bool bLeanRight;
	UPROPERTY()
	float LeanAmount;
	UPROPERTY()
	float LeanSpeed;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float LeanMultiplier = 15.f;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FRotator LeanRotator;
	
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FTransform LeftHandSocketTransform;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> DieMontageArray;
};