// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSession/EOSGameSession.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

void AEOSGameSession::BeginPlay()
{
	Super::BeginPlay();

	if (IsRunningDedicatedServer() && !bSessionExists)
	{
		CreateSession("KeyName", "KeyValue");
	}
}

bool AEOSGameSession::ProcessAutoLogin()
{
	return true;
}

void AEOSGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
	Super::NotifyLogout(ExitingPlayer);

	if (IsRunningDedicatedServer())
	{
		if (--NumberOfPlayersInSession == 0)
		{
			EndSession();
		}
	}
}

void AEOSGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

	if (IsRunningDedicatedServer())
	{
		IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
		check(Subsystem);
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

		RegisterPlayerDelegateHandle =
			Session->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
				this,
				&AEOSGameSession::HandleRegisterPlayerCompleted));

		if (!Session->RegisterPlayer(SessionName, *UniqueId, false))
		{
			UE_LOG(LogTemp, Warning, TEXT("Falied to Register Player!"));
			Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
			RegisterPlayerDelegateHandle.Reset();
		}
	}
}

void AEOSGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	Super::UnregisterPlayer(ExitingPlayer);

	if (IsRunningDedicatedServer())
	{
		IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
		check(Subsystem);
		IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

		if (ExitingPlayer->PlayerState)
		{
			UnregisterPlayerDelegateHandle =
				Session->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(
					this,
					&AEOSGameSession::HandleUnregisterPlayerCompleted));

			if (!Session->UnregisterPlayer(SessionName, *ExitingPlayer->PlayerState->GetUniqueId()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Falied to Unregister Player!"));
				Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
				UnregisterPlayerDelegateHandle.Reset();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Falied to Unregister Player!"));
			Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
			UnregisterPlayerDelegateHandle.Reset();
		}
	}
}

void AEOSGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsRunningDedicatedServer())
	{
		DestroySession();
	}
}

void AEOSGameSession::CreateSession(FName KeyName, FString KeyValue)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	CreateSessionDelegateHandle =
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&AEOSGameSession::HandleCreateSessionCompleted));

	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();

	SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = false;
	SessionSettings->bAllowJoinViaPresence = false;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bAllowInvites = false;
	SessionSettings->bAllowJoinInProgress = false;
	SessionSettings->bIsDedicated = true;
	SessionSettings->bUseLobbiesIfAvailable = false;
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = false;
	SessionSettings->bUsesStats = true;

	SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

	UE_LOG(LogTemp, Log, TEXT("Creating session..."));

	if (!Session->CreateSession(0, SessionName, *SessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
		Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
		CreateSessionDelegateHandle.Reset();
	}
}

void AEOSGameSession::StartSession()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	StartSessionDelegateHandle =
		Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(
			this,
			&AEOSGameSession::HandleStartSessionCompleted));

	if (!Session->StartSession(SessionName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Falied to start session!"));
		Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
		StartSessionDelegateHandle.Reset();
	}
}

void AEOSGameSession::EndSession()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	EndSessionDelegateHandle =
		Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionCompleteDelegate::CreateUObject(
			this,
			&AEOSGameSession::HandleEndSessionCompleted));

	if (!Session->EndSession(SessionName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to end session!"));
		Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
		EndSessionDelegateHandle.Reset();
	}
}

void AEOSGameSession::DestroySession()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	DestroySessionDelegateHandle =
		Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(
			this,
			&AEOSGameSession::HandleDestroySessionCompleted));

	if (!Session->DestroySession(SessionName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session."));
		Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
		DestroySessionDelegateHandle.Reset();
	}
}


void AEOSGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		bSessionExists = true;
		UE_LOG(LogTemp, Log, TEXT("Session : %s Created!"), *EOSSessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
	}
	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
	CreateSessionDelegateHandle.Reset();
}

void AEOSGameSession::HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Started!"));
		GetWorld()->ServerTravel(FString("/Game/ThirdPerson/Maps/ThirdPersonMap"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Falied to start session! (From Callback)"));
	}

	Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
	StartSessionDelegateHandle.Reset();
}

void AEOSGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session ended!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to end session! (From Callback)"));
	}
	Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
	EndSessionDelegateHandle.Reset();
}

void AEOSGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy session successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session."));	
	}
	
	Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
	DestroySessionDelegateHandle.Reset();
}

void AEOSGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName,
                                                    const TArray<FUniqueNetIdRef>& PlayerId, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Player registered in EOS Session!"));
		if (++NumberOfPlayersInSession == MaxNumberOfPlayersInSession)
		{
			StartSession();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Falied to register player! (From Callback)")); 
		}

		Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
		RegisterPlayerDelegateHandle.Reset();
	}
}

void AEOSGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerId,
	bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	check(Subsystem);
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Player unregistered in EOS Session!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to unregister player! (From Callback)"));
	}
	Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
	UnregisterPlayerDelegateHandle.Reset();
}
