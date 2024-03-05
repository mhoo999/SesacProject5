// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

class AMainMenuPlayerController;
class UGameLoadingUI;
class UTitleUI;
class UButton;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	AMainMenuPlayerController* pc;
	
	UFUNCTION()
	void OpenInventory();
	UFUNCTION()
	void OpenCityTab();
	UFUNCTION()
	void MovetoField();
	UFUNCTION()
	void OpenWorkBenchTab();
	UFUNCTION()
	void MovetoTitle();
};