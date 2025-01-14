// Copyright Jonathan Cote


#include "AbilitySystem/Abilities/RpgDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayCueNotifyTypes.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"

void URpgDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == TargetActor) { return; }
	if (!URpgAbilitySystemLibrary::IsNotFriendly(SourceAvatarActor, TargetActor)) { return; }
		
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		URpgAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}

void URpgDamageGameplayAbility::CauseRadialDamage(const FVector& RadialDamageOrigin, const TArray<AActor*>& ActorsToIgnore)
{
	TArray<AActor*> OverlappingActors;
	URpgAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlappingActors, ActorsToIgnore, DamageInfo.RadialDamageInfo.RadialDamageOuterRadius, RadialDamageOrigin);

	for (AActor* TargetActor : OverlappingActors)
	{
		FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		DamageEffectParams.DamageInfo.RadialDamageInfo.RadialDamageOrigin = RadialDamageOrigin;
		const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
		if (SourceAvatarActor != TargetActor && URpgAbilitySystemLibrary::IsNotFriendly(SourceAvatarActor, TargetActor))
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
			{
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				URpgAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}
	}
}


FDamageEffectParams URpgDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageInfo = DamageInfo;
	Params.DebuffInfo = DebuffInfo;
	return Params;
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
