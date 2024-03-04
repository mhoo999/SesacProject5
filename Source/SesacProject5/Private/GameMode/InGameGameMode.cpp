// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/InGameGameMode.h"

#include "EngineUtils.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Component/EscapeComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Object/PlayerStartBase.h"

void AInGameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::InitGame"));
	
	for (TActorIterator<APlayerStartBase> It(GetWorld()); It; ++It)
	{
		PlayerStartArray.Add(*It);
	}
}

AActor* AInGameGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::ChoosePlayerStart) PlayerStartArray Num : %d"), SpawnPointArray.Num());
	if (PlayerStartArray.Num() <= 0) return nullptr;

	APlayerStart* PlayerStart = nullptr;
	while (PlayerStartArray.Num())
	{
		int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num()-1);
		if (PlayerStartArray[RandIndex]->IsOccupied())
		{
			PlayerStartArray.RemoveAt(RandIndex);
		}
		else
		{
			PlayerStart = Cast<APlayerStart>(PlayerStartArray[RandIndex]->SetOccupied());
		}
	}
	
	// AActor* PlayerStart = Super::ChoosePlayerStart_Implementation(Player);
	// UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::ChoosePlayerStart) %s"), (PlayerStart ? *PlayerStart->GetActorNameOrLabel() : *FString("Empty")));
	
	return PlayerStart;
}

void AInGameGameMode::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
	Super::InitStartSpot_Implementation(StartSpot, NewPlayer);

	// UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode::InitStartSpot) Pawn Name : %s"), *NewPlayer->GetPawn()->GetActorNameOrLabel());
	if (UEscapeComponent* EscapeComponent = NewPlayer->GetPawn()->GetComponentByClass<UEscapeComponent>())
	{
		Cast<APlayerStartBase>(StartSpot)->SetExitArrayToComponent(EscapeComponent);
	}
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
