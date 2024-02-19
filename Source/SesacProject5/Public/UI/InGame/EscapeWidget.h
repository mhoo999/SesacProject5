// copyright by LootingEmployment

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeWidget.generated.h"

class UBorder;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SESACPROJECT5_API UEscapeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(APawn* Pawn);

	void UpdateIsEscaping(bool bNewIsEscaping);
	void UpdateEscapeTime(float NewEscapeTime);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_EscapeTime;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_EscapeTime;
};