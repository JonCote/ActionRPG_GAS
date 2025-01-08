// Copyright Jonathan Cote


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "RpgAbilityTypes.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "AbilitySystem/RpgAttributeSet.h"

struct RpgTargetStatics
{
	// Capture from Target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurseResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	
	RpgTargetStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CurseResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, PhysicalResistance, Target, false);

	}
};

static const RpgTargetStatics& TargetDamageStatics()
{
	static RpgTargetStatics TrgDmgStatics;
	return TrgDmgStatics;
}


struct RpgSourceDamageStatics
{
	// Capture from Source
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitRate)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	
	RpgSourceDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitRate, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, MaxHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, Defense, Source, true);
		
	}
};

static const RpgSourceDamageStatics& SourceDamageStatics()
{
	static RpgSourceDamageStatics SrcDmgStatics;
	return SrcDmgStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	// Target Attribute Capture
	RelevantAttributesToCapture.Add(TargetDamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().CurseResistanceDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(TargetDamageStatics().PhysicalResistanceDef);

	
	// Source Attribute Capture
	RelevantAttributesToCapture.Add(SourceDamageStatics().CriticalHitRateDef);
	RelevantAttributesToCapture.Add(SourceDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(SourceDamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(SourceDamageStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(SourceDamageStatics().DefenseDef);
	
}


/*
 * Debuff Logic
**/
void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& SourceTagsToCaptureDefs,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TargetTagsToCaptureDefs) const
{
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get(); 
	
	for (const FGameplayTag& DebuffTypeTag : GameplayTags.DebuffTypes)
	{
		const float DebuffValid = Spec.GetSetByCallerMagnitude(DebuffTypeTag, false, -1.f);
		// Check if a positive value was set for this DebuffType
		if (DebuffValid > 0.f)
		{
			// Determine if Debuff is successful
			float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Stats_Chance, false, -1.f);
			SourceDebuffChance = FMath::Clamp(SourceDebuffChance, 0.f, 100.f);
			const bool bDebuffSuccess = FMath::RandRange(1, 100) <= SourceDebuffChance;
			if (bDebuffSuccess)
			{
				
				//float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Stats_Damage, false);
				float DebuffDamage = 0.f;
				float TargetResistance = 0.f;
				for (const auto& Pair : FRpgGameplayTags::Get().DebuffDamageTypesToResistances)
				{
					const FGameplayTag& DamageTypeTag = Pair.Key;
					const FGameplayTag& ResistanceTag = Pair.Value;
		
					checkf(TargetTagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
					const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TargetTagsToCaptureDefs[ResistanceTag];

					// Add the base damage for Debuff to Damage Total
					DebuffDamage += Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
					float Resistance = 0.f;
					ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, Resistance);
					TargetResistance = FMath::Clamp(Resistance, 0.f, 100.f);
				}
				// Add Source Attribute Damage Multiplier Calculations results to Damage
				DebuffDamage += CalculateMultiplierDamage(ExecutionParams, Spec, EvalParams, SourceTagsToCaptureDefs, FRpgGameplayTags::Get().DebuffDamageMultipliersToSourceAttributes);

				// Add Target Attribute Damage Multipliers Calculations results to Damage
				DebuffDamage += CalculateMultiplierDamage(ExecutionParams, Spec, EvalParams, TargetTagsToCaptureDefs, FRpgGameplayTags::Get().DebuffDamageMultipliersToTargetAttributes);
				
				// Apply TargetResistance
				DebuffDamage *= (100.f - TargetResistance) / 100.f;

				// Round to nearest int value
				DebuffDamage += 0.5f;
				DebuffDamage = FMath::FloorToFloat(DebuffDamage);

				float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Stats_Frequency, false);
				
				float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Stats_Duration, false);

				//TODO: Add Tenacity (Crowd Control Reduction) Attribute to game and capture it here that will reduce duration of certain types of Debuffs
				if (GameplayTags.DebuffsEffectedByTenacity.Contains(DebuffTypeTag))
				{
					float TargetTenacity = 0.f;
					TargetTenacity = FMath::Clamp(TargetTenacity, 0.f, 50.f);  // 50% Tenacity is max value
					DebuffDuration = DebuffDuration * (100.f - TargetTenacity) / 100.f;
				}
				
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				URpgAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, bDebuffSuccess);
				URpgAbilitySystemLibrary::SetDebuffTag(ContextHandle, DebuffTypeTag);
				URpgAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				URpgAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				URpgAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}


/*
 * Calculate Damage
 **/
float UExecCalc_Damage::CalculateDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& SourceTagsToCaptureDefs,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TargetTagsToCaptureDefs) const
{
	float Damage = 0.f;

	// Add Source Attribute Damage Multiplier Calculations results to Damage
	Damage += CalculateMultiplierDamage(ExecutionParams, Spec, EvalParams, SourceTagsToCaptureDefs, FRpgGameplayTags::Get().DamageMultipliersToSourceAttributes);

	// Add Target Attribute Damage Multipliers Calculations results to Damage
	Damage += CalculateMultiplierDamage(ExecutionParams, Spec, EvalParams, TargetTagsToCaptureDefs, FRpgGameplayTags::Get().DamageMultipliersToTargetAttributes);

	// Add BaseDamage to Damage, and get TargetResistance value corresponding to DamageType
	float TargetResistance = 0.f;
	for (const auto& Pair : FRpgGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& ResistanceTag = Pair.Value;
		
		checkf(TargetTagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TargetTagsToCaptureDefs[ResistanceTag];

		// Add the base damage for spell to Damage Total
		Damage += Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, Resistance);
		TargetResistance = FMath::Clamp(Resistance, 0.f, 100.f);
	}

	// Determine if a Critical Hit was caused using Source's Critical Hit Rate, and Get Source's CritDamage Multiplier value
	float CritRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SourceDamageStatics().CriticalHitRateDef, EvalParams, CritRate);
	CritRate = FMath::Clamp(CritRate, 0.f, 100.f);

	const bool bCritSuccess = FMath::RandRange(1, 100) <= CritRate;
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	URpgAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCritSuccess);
	
	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SourceDamageStatics().CriticalHitDamageDef, EvalParams, CritDamage);
	
	// Calculate DamageReduction based on Targets Defense
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TargetDamageStatics().DefenseDef, EvalParams, Defense);
	Defense = FMath::Max<float>(0.f, Defense);
	const float DamageReduction = Defense * 0.1f;

	/* Calculate Final Damage Applied to Target */
	// Apply DamageReduction
	Damage -= FMath::Max<float>(0.f, DamageReduction);

	// Apply CritDamage Multiplier
	if (bCritSuccess)
	{
		Damage *= CritDamage;
	}

	// Apply TargetResistance
	Damage *= (100.f - TargetResistance) / 100.f;

	// Round to nearest int value
	Damage += 0.5f;
	return FMath::FloorToFloat(Damage);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/*const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;*/
	
	const FRpgGameplayTags& Tags = FRpgGameplayTags::Get();
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> SourceTagsToCaptureDefs;
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TargetTagsToCaptureDefs;
	// Target
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, TargetDamageStatics().DefenseDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Secondary_MaxHealth, TargetDamageStatics().MaxHealthDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, TargetDamageStatics().FireResistanceDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, TargetDamageStatics().LightningResistanceDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Resistance_Curse, TargetDamageStatics().CurseResistanceDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice, TargetDamageStatics().IceResistanceDef);
	TargetTagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, TargetDamageStatics().PhysicalResistanceDef);
	// Source
	SourceTagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitRate, SourceDamageStatics().CriticalHitRateDef);
	SourceTagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, SourceDamageStatics().CriticalHitDamageDef);
	SourceTagsToCaptureDefs.Add(Tags.Attributes_Secondary_AttackPower, SourceDamageStatics().AttackPowerDef);
	SourceTagsToCaptureDefs.Add(Tags.Attributes_Secondary_MaxHealth, SourceDamageStatics().MaxHealthDef);
	SourceTagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, SourceDamageStatics().DefenseDef);
	
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	/*
	 * Calculate Damage
	**/
	const float Damage = CalculateDamage(ExecutionParams, Spec, EvalParams, SourceTagsToCaptureDefs, TargetTagsToCaptureDefs);


	/*
	 * Debuff
	**/
	DetermineDebuff(ExecutionParams, Spec, EvalParams, SourceTagsToCaptureDefs, TargetTagsToCaptureDefs);

	
	const FGameplayModifierEvaluatedData EvalData(URpgAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvalData);
}

float UExecCalc_Damage::CalculateMultiplierDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs,
	const TMap<FGameplayTag, FGameplayTag>& DamageMultiplierToAttribute) const
{
	float Damage = 0.0f;
	for (const auto& Pair : DamageMultiplierToAttribute)
	{
		const FGameplayTag& MultiplierTag = Pair.Key;
		const FGameplayTag& AttributeTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(AttributeTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *AttributeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs[AttributeTag];
		
		const float MultiplierValue = Spec.GetSetByCallerMagnitude(MultiplierTag, false);

		float AttributeValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, AttributeValue);
		AttributeValue = FMath::Max(0.f, AttributeValue);

		Damage += MultiplierValue * AttributeValue;
	}
	return Damage;
}
