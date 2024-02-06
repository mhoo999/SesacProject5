// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EFSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UEFSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	FString GetPlayerUserName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	bool IsPlayerLoggedIn();

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};