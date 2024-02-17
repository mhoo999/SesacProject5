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

	virtual void NativeConstruct() override;

public:
	// -------------------------------- main menu -------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_gameStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_exit;

	UFUNCTION()
	void MovetoLobby();
	UFUNCTION()
	void ExitGame();

public:
	// -------------------------------- lobby -------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_city;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_movetoField;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_workBench;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* btn_back;
	
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
