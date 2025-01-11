// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
						const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& SourceTagsToCaptureDefs,
						const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TargetTagsToCaptureDefs) const;

	

	
	float CalculateDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						 const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& SourceTagsToCaptureDefs,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TargetTagsToCaptureDefs) const;

	float CalculateMultiplierDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
									const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvalParams,
									const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs,
									const TMap<FGameplayTag, FGameplayTag>& DamageMultiplierToAttribute) const;
};
