// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthSlotWidget.generated.h"

enum class EBodyParts : uint8;
class UHealthComponent;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UHealthSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void InitWidget(UHealthComponent* HealthComponent);
	
	UFUNCTION()
	void UpdateWidget(float Health, float MaxHealth);
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	EBodyParts BodyParts;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_SlotName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_Health;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_MaxHealth;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UProgressBar* PB_Health;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	FText SlotName;
};