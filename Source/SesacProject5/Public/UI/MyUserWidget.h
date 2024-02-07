// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UWidgetSwitcher* widgetSwitcher;


public:
	// -------------------------------- main menu -------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_gameStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_exit;

	UFUNCTION()
	void gotoLobby();

public:
	// -------------------------------- lobby -------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_city;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_movetoSesac;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_workBench;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_back;
};
