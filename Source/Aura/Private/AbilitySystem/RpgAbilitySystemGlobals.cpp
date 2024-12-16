// Copyright Jonathan Cote


#include "AbilitySystem/RpgAbilitySystemGlobals.h"

#include "RpgAbilityTypes.h"

FGameplayEffectContext* URpgAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRpgGameplayEffectContext();
}
