// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_AttackPower.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_AttackPower : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_AttackPower();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override; 
	
private:

	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;

	const float BaseAttackPower = 10.f;
	const float StrengthCoefficient = 3.f;
	const float IntelligenceCoefficient = 1.5f;
	const float LevelCoefficient = 2.f;
};
