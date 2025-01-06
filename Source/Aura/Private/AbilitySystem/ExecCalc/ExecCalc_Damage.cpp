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
	

	// GameplayTag to AttributeCaptureDefinition
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	RpgTargetStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CurseResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, PhysicalResistance, Target, false);

		const FRpgGameplayTags& Tags = FRpgGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, DefenseDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_MaxHealth, MaxHealthDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Curse, CurseResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice, IceResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
		
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

	// GameplayTag to AttributeCaptureDefinition
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	RpgSourceDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitRate, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, MaxHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, Defense, Source, true);

		const FRpgGameplayTags& Tags = FRpgGameplayTags::Get();
		
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitRate, CriticalHitRateDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_AttackPower, AttackPowerDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_MaxHealth, MaxHealthDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, DefenseDef);
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

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;

	// Damage Scaled by Source Attribute value
	for (const auto& Pair : FRpgGameplayTags::Get().DamageMultipliersToSourceAttributes)
	{
		const FGameplayTag& MultiplierTag = Pair.Key;
		const FGameplayTag& AttributeTag = Pair.Value;

		checkf(RpgSourceDamageStatics().TagsToCaptureDefs.Contains(AttributeTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *AttributeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = RpgSourceDamageStatics().TagsToCaptureDefs[AttributeTag];
		
		float MultiplierValue = Spec.GetSetByCallerMagnitude(MultiplierTag, false);

		float AttributeValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, AttributeValue);
		AttributeValue = FMath::Max(0.f, AttributeValue);

		Damage += MultiplierValue * AttributeValue;
	}

	// Damage Scaled by Target Attribute value
	for (const auto& Pair : FRpgGameplayTags::Get().DamageMultipliersToTargetAttributes)
	{
		const FGameplayTag& MultiplierTag = Pair.Key;
		const FGameplayTag& AttributeTag = Pair.Value;

		checkf(RpgTargetStatics().TagsToCaptureDefs.Contains(AttributeTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *AttributeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = RpgTargetStatics().TagsToCaptureDefs[AttributeTag];
		
		float MultiplierValue = Spec.GetSetByCallerMagnitude(MultiplierTag, false);

		float AttributeValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, AttributeValue);
		AttributeValue = FMath::Max(0.f, AttributeValue);

		Damage += MultiplierValue * AttributeValue;
	}

	// Base Elemental Damage added to Total Damage, and get TargetResistance value to damage type
	float TargetResistance = 0.f;
	for (const auto& Pair : FRpgGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& ResistanceTag = Pair.Value;
		
		checkf(RpgTargetStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = RpgTargetStatics().TagsToCaptureDefs[ResistanceTag];

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

	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SourceDamageStatics().CriticalHitDamageDef, EvalParams, CritDamage);
	const bool bCrit = FMath::RandRange(1.f, 100.f) <= CritRate;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	URpgAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCrit);
	
	// Calculate DamageReduction based on Targets Defense
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TargetDamageStatics().DefenseDef, EvalParams, Defense);
	Defense = FMath::Max<float>(0.f, Defense);
	const float DamageReduction = Defense * 0.1f;

	/* Calculate Final Damage Applied to Target */
	// Apply DamageReduction
	Damage -= FMath::Max<float>(0.f, DamageReduction);

	// Apply CritDamage Multiplier
	if (bCrit)
	{
		Damage *= CritDamage;
	}

	// Apply TargetResistance
	Damage *= (100.f - TargetResistance) / 100.f;

	// Round to nearest int value
	Damage += 0.5f;
	Damage = FMath::FloorToFloat(Damage);
	
	const FGameplayModifierEvaluatedData EvalData(URpgAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvalData);
}
