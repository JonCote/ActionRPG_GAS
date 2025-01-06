// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "RpgDamageGameplayAbility.generated.h"

struct FDamageInfo
{
	FGameplayTag DamageTypeTag = FGameplayTag();
	FScalableFloat BaseDamage = 0.0f;
	TMap<FGameplayTag, FScalableFloat> DamagesMultipliers = TMap<FGameplayTag, FScalableFloat>();
};

/**
 * 
 */
UCLASS()
class AURA_API URpgDamageGameplayAbility : public URpgGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageMultipliers;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetDamageByDamageType(const FGameplayTag& DamageType, const int32 InLevel) const;
	float GetDamageMultiplierByTag(const FGameplayTag& MultiplierTag, const int32 InLevel) const;
	
	
};
