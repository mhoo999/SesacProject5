// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGameMode.generated.h"

class APlayerStartBase;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API AInGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<APlayerStartBase*> PlayerStartArray; 
};