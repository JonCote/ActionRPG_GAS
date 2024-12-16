// Copyright Jonathan Cote


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "RpgAbilityTypes.h"
#include "RpgGameplayTags.h"
#include "RpgAbilityTypes.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "AbilitySystem/RpgAttributeSet.h"

struct RpgDamageStatics
{
	// Capture from Target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurseResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	// Capture from Source
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitRate)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	// GameplayTag to AttributeCaptureDefinition
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	RpgDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CurseResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, PhysicalResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitRate, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URpgAttributeSet, CriticalHitDamage, Source, true);

		const FRpgGameplayTags& Tags = FRpgGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Defense, DefenseDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Curse, CurseResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice, IceResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
		
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitRate, CriticalHitRateDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
	}
};

static const RpgDamageStatics& DamageStatics()
{
	static RpgDamageStatics DmgStatics;
	return DmgStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CurseResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
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
	for (const auto& Pair : FRpgGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& ResistanceTag = Pair.Value;
		
		checkf(RpgDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = RpgDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvalParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Determine if a Critical Hit was caused using Source's Critical Hit Rate
	float CritRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitRateDef, EvalParams, CritRate);
	CritRate = FMath::Clamp(CritRate, 0.f, 100.f);

	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvalParams, CritDamage);
	const bool bCrit = FMath::RandRange(1.f, 100.f) <= CritRate;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	URpgAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCrit);
	
	// Reduce the Damage based on targets Defense
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvalParams, Defense);
	Defense = FMath::Max<float>(0.f, Defense);
	const float DamageReduction = Defense * 0.1f;

	if (bCrit)
	{
		Damage = FMath::Max<float>(0.f, ((Damage * CritDamage) - DamageReduction));
	}
	else
	{
		Damage = FMath::Max<float>(0.f, (Damage - DamageReduction));
	}

	Damage = FMath::CeilToFloat(Damage);
	
	const FGameplayModifierEvaluatedData EvalData(URpgAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvalData);
}
