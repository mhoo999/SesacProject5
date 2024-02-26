// copyright by LootingEmployment


#include "AnimInstance/FPSAnimInstance.h"

#include "Component/HealthComponent.h"

void UFPSAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	GetOwningActor()->GetComponentByClass<UHealthComponent>()->OnIsDeadChanged.AddUObject(this, &UFPSAnimInstance::Die);
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (bIsDead) return;
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UFPSAnimInstance::Die(bool bNewIsDead)
{
	bIsDead = bNewIsDead;

	if (bIsDead)
	{
		if (DieMontageArray.Num() > 0)
		{
			Montage_Play(DieMontageArray[FMath::RandRange(0, DieMontageArray.Num()-1)]);
		}
	}
}