// copyright by LootingEmployment


#include "Object/PlayerStartBase.h"

#include "Component/EscapeComponent.h"

void APlayerStartBase::SetExitArrayToComponent(UEscapeComponent* EscapeComponent)
{
	if (ParentSpawnPoint)
	{
		ParentSpawnPoint->SetExitArrayToComponent(EscapeComponent);
	}
	else
	{
		EscapeComponent->SetExitArray(ExitArray);
	}
}

bool APlayerStartBase::IsOccupied() const
{
	if (ExitArray.Num() > 0)
	{
		for (auto Iter : SpawnPointArray)
		{
			if (Iter->IsOccupied() == false) return false;
		}
	}
		
	return bIsOccupied;
}

ISpawnPoint* APlayerStartBase::SetOccupied()
{
	if (ExitArray.Num() > 0)
	{
		for (auto Iter : SpawnPointArray)
		{
			if (Iter->IsOccupied() == false)
			{
				SetParent(this);
				return Iter->SetOccupied();
			}
		}
	}

	if (false == bIsOccupied)
	{
		bIsOccupied = true;
		return this;
	}

	return nullptr;
}

void APlayerStartBase::SetParent(ISpawnPoint* NewParent)
{
	ParentSpawnPoint = NewParent;
}