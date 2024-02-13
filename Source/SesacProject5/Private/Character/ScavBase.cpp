// copyright by LootingEmployment


#include "Character/ScavBase.h"

#include "Component/FSMComponent.h"

AScavBase::AScavBase()
{
	FSMComp = CreateDefaultSubobject<UFSMComponent>(FName("FSM Component"));
}
