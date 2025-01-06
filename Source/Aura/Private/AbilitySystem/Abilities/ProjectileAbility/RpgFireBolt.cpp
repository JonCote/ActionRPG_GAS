// Copyright Jonathan Cote


#include "AbilitySystem/Abilities/ProjectileAbility/RpgFireBolt.h"

#include "RpgGameplayTags.h"


FString URpgFireBolt::GetDescription(const FString& AbilityName, const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float BaseDamage = GetBaseDamage(Level);
	
	TMap<FString, float> Multipliers;
	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageInfo.DamageMultipliers)
	{
		if (Pair.Key.MatchesTagExact(GameplayTags.DamageMultiplier_Source_AttackPower))
		{
			Multipliers.Add(FString("AttackPower"), GetDamageMultiplierByTag(Pair.Key, Level) * 100.f);
		}
	}
	
	FString Description = FString();
	Description += FString::Printf(TEXT("<Title>")) + AbilityName + FString::Printf(TEXT("</>\n"));
	Description += FString::Printf(TEXT("<Small>Level: </><Level>%d</>\n<Small>Mana Cost: </><Mana>%d</>\n<Small>Cooldown: </><Cooldown>%.2f</><Small>sec</>\n\n"), Level, static_cast<int>(ManaCost), Cooldown);
	Description += FString::Printf(TEXT("<Default>Base Damage: </><BaseDamage>%d</>\n"), static_cast<int>(BaseDamage));
	for (auto& Pair : Multipliers)
	{
		Description += FString::Printf(TEXT("<Default>")) + Pair.Key + FString::Printf(TEXT(" Multiplier </><")) + Pair.Key + FString::Printf(TEXT(">%d%%</>\n"), static_cast<int>(Pair.Value));
	}

	
	return Description;
}
