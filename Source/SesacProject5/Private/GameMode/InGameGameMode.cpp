// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/InGameGameMode.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"

AInGameGameMode::AInGameGameMode()
{
}

AActor* AInGameGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::ChoosePlayerStart) %s"), *Player->GetActorNameOrLabel());
	return Super::ChoosePlayerStart_Implementation(Player);
}

AActor* AInGameGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::FindPlayerStart) %s"), *Player->GetActorNameOrLabel());
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}

void AInGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// if (NewPlayer == nullptr || NewPlayer->IsValidLowLevelFast() == false) return;
	// FUniqueNetIdRepl UniqueNetIdRepl;
	//
	// if (NewPlayer->IsLocalController())
	// {
	// 	ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
	// 	if (LocalPlayerRef)
	// 	{
	// 		UniqueNetIdRepl = LocalPlayerRef->GetPreferredUniqueNetId();
	// 	}
	// 	else
	// 	{
	// 		UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
	// 		check(IsValid(RemoteNetConnectionRef));
	//
	// 		UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
	// 	}
	// }
	// else
	// {
	// 	UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
	// 	check(IsValid(RemoteNetConnectionRef));
	//
	// 	UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
	// }
	//
	// TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
	//
	// IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
	// IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
	//
	// bool bRegistrationSuccess = SessionRef->RegisterPlayer(FName("MainSession"), *UniqueNetId, false);
	// if (bRegistrationSuccess)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::PostLogin) Registration Successful : %s"), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName()); 
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("AInGameGameMode::PostLogin) Registration Failed : %s"), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName());
	// }
}
