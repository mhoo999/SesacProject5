// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UOpeningWidget;
class UGameLoadingUI;
class ULobbyUI;
class UTitleUI;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UOpeningWidget> openingWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UTitleUI> titleUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<ULobbyUI> lobbyUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UISettings", meta=(AllowPrivateAccess))
	TSubclassOf<UGameLoadingUI> gameLoadingUI;

	UOpeningWidget* opening;
	UTitleUI* title;
	ULobbyUI* lobby;
	UGameLoadingUI* gameLoading;
	
public:
	UFUNCTION(BlueprintCallable)
	void HiddenOpening();
	
	UFUNCTION(BlueprintCallable)
	void ShowTitle();
	UFUNCTION(BlueprintCallable)
	void HiddenTitle();

	UFUNCTION(BlueprintCallable)
	void ShowLobby();
	UFUNCTION(BlueprintCallable)
	void HiddenLobby();
	
	UFUNCTION(BlueprintCallable)
	void ShowGameLoading();
	UFUNCTION(BlueprintCallable)
	void HiddenGameLoading();
};
