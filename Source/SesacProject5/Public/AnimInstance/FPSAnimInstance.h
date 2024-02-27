// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "FPSAnim_AnimInstance.h"
#include "FPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UFPSAnimInstance : public UFPSAnim_AnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UFUNCTION()
	void Die(bool bNewIsDead);

private:
	UPROPERTY()
	bool bIsDead = false;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	bool bIsCrouch = false;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> DieMontageArray;
};