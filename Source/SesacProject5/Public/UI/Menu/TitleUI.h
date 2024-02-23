// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUI.generated.h"

class AMainMenuPlayerController;
class ULobbyUI;
class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UTitleUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	AMainMenuPlayerController* pc;

	UFUNCTION()
	void MovetoLobby();
	UFUNCTION()
	void ExitGame();
};
