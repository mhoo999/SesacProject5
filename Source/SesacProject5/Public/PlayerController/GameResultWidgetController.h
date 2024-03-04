// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameResultWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AGameResultWidgetController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> resultWidget;

	UUserWidget* result;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UAnimMontage> successMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UAnimMontage> failMontage;

	void playSuccessMontage();
	void playFailMontage();
};
