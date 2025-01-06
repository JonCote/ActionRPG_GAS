// Copyright Jonathan Cote


#include "AbilitySystem/Abilities/RpgGameplayAbility.h"

#include "AbilitySystem/RpgAttributeSet.h"

FString URpgGameplayAbility::GetDescription(const FString& AbilityName, int32 Level)
{
	return FString::Printf(TEXT("<Title>%s</>\n"), *AbilityName);
}

FString URpgGameplayAbility::GetLockedDescription(const FString& AbilityName, const int32 Level)
{
	return FString::Printf(TEXT("<Title>%s</>\n<Default>Unlock at Level: </><Level>%d</>\n"), *AbilityName, Level);
}

float URpgGameplayAbility::GetManaCost(const int32 InLevel) const
{
	float Cost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod: CostEffect->Modifiers)
		{
			if (Mod.Attribute == URpgAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cost);
				break;
			}
		}
	}
	
	return Cost; 
}

float URpgGameplayAbility::GetCooldown(const int32 InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
