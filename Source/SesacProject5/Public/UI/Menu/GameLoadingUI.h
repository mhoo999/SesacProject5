// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoadingUI.generated.h"

class AMainMenuPlayerController;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UGameLoadingUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	AMainMenuPlayerController* pc;
};
