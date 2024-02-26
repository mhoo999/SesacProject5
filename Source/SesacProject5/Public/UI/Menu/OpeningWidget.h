// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningWidget.generated.h"

class AMainMenuPlayerController;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UOpeningWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	AMainMenuPlayerController* pc;

	UFUNCTION(BlueprintCallable)
	void PressAnyKey();
};
