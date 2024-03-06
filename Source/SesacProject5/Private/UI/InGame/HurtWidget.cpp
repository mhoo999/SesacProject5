// copyright by LootingEmployment


#include "UI/InGame/HurtWidget.h"

#include "Component/HealthComponent.h"
#include "Components/Image.h"

void UHurtWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Img_Blood->SetOpacity(FMath::Clamp(Img_Blood->GetColorAndOpacity().A - InDeltaTime, 0.f, 1.f));
}

void UHurtWidget::InitWidget(APawn* Pawn)
{
	if (UHealthComponent* HealthComponent = Pawn->GetComponentByClass<UHealthComponent>())
	{
		HealthComponent->GetHealth(EBodyParts::HEAD).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::THORAX).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::STOMACH).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::LEFTARM).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::RIGHTARM).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::LEFTLEG).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
		HealthComponent->GetHealth(EBodyParts::RIGHTLEG).OnHealthChanged.AddUObject(this, &UHurtWidget::UpdateHealthChange);
	}
}

void UHurtWidget::ShowHurtImage()
{
	Img_Blood->SetOpacity(FMath::Clamp(Img_Blood->GetColorAndOpacity().A + IncreaseAmount, 0.f, 1.f));
}

void UHurtWidget::UpdateHealthChange(EBodyParts BodyParts, float CurrentHealth, float MaxHealth)
{
	ShowHurtImage();
}