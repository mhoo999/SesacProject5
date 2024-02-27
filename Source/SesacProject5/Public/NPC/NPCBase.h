// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCBase.generated.h"

class UImage;

UCLASS()
class SESACPROJECT5_API ANPCBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ANPCBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="NPC", meta=(AllowPrivateAccess))
	FName name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="NPC", meta=(AllowPrivateAccess))
	UTexture2D* portrait;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="NPC", meta=(AllowPrivateAccess))
	UDataTable* questDataTable;

	// selling item
};
