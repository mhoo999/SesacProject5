// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HurtWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class SESACPROJECT5_API UHurtWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitWidget(APawn* Pawn);

	void ShowHurtImage();

	UFUNCTION()
	void UpdateHealthChange(EBodyParts BodyParts, float CurrentHealth, float MaxHealth);
	
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_Blood;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float IncreaseAmount = 0.3f;
};