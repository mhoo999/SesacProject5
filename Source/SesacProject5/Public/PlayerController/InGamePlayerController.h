// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

class UInGameWidget;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ToggleTabWidget();
protected:
	//virtual void OnNetCleanup(UNetConnection* Connection) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UInGameWidget> InGameWidgetClass;
	UPROPERTY()
	UInGameWidget* InGameWidget;
};