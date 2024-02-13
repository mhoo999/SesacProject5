// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "EFSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UEFSGameInstance : public UGameInstance
{
GENERATED_BODY()

// public:
// 	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
// 	void LoginWithEOS(FString ID, FString Token, FString LoginType);
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
// 	FString GetPlayerUserName();
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
// 	bool IsPlayerLoggedIn();
//
// 	// Session
// 	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
// 	void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);
// 	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
// 	void FindSessionAndJoin();
// 	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
// 	void JoinEOSSession();
// 	UFUNCTION(BlueprintCallable, Category = "EOS Functions")
// 	void DestroySession();
//
// 	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
// 	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
// 	void OnFindSessionComplete(bool bWasSuccess);
// 	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
// 	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
//
// private:
// 	UPROPERTY(EditDefaultsOnly, Category = "EOS Variables", Meta = (AllowPrivateAccess))
// 	FString LevelURL;
// 	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};