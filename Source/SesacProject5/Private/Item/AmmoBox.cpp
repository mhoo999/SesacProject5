// copyright by LootingEmployment


#include "Item/AmmoBox.h"

#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"

void AAmmoBox::Interact(ACharacter* InteractCharacter, FText InteractionName)
{
	Super::Interact(InteractCharacter, InteractionName);

	FString Interaction = InteractionName.ToString();

	if (Interaction.Equals("GetAmmo"))
	{
		if (UWeaponComponent* WeaponComponent = InteractCharacter->GetComponentByClass<UWeaponComponent>())
		{
			WeaponComponent->AddAmmo(AmmoCount);	
		}
		Destroy();
	}
}