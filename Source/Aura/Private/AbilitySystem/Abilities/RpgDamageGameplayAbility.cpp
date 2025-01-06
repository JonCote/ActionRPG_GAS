// Copyright Jonathan Cote


#include "AbilitySystem/Abilities/RpgDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RpgGameplayTags.h"

void URpgDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageInfo.DamageMultipliers)
	{
		float ScaledMultiplier = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledMultiplier);
	}
	float ScaledDamage = DamageInfo.BaseDamage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageInfo.DamageTypeTag, ScaledDamage);
	
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FTaggedMontage URpgDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() == 0) { return FTaggedMontage(); }

	const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);

	return TaggedMontages[Selection];
}

float URpgDamageGameplayAbility::GetBaseDamage(const int32 InLevel) const
{
	return DamageInfo.BaseDamage.GetValueAtLevel(InLevel);
}

float URpgDamageGameplayAbility::GetDamageMultiplierByTag(const FGameplayTag& MultiplierTag, const int32 InLevel) const
{
	checkf(DamageInfo.DamageMultipliers.Contains(MultiplierTag), TEXT("GameplayAbility [%s] does not contain MultiplierTag [%s]"), *GetNameSafe(this), *MultiplierTag.ToString());
	return DamageInfo.DamageMultipliers[MultiplierTag].GetValueAtLevel(InLevel);
}
