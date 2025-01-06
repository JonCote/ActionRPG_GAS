// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "RpgDamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat BaseDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FScalableFloat> DamageMultipliers = TMap<FGameplayTag, FScalableFloat>();
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

	UPROPERTY(EditDefaultsOnly, Category = "DamageInfo")
	FDamageInfo DamageInfo;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetBaseDamage(const int32 InLevel) const;
	float GetDamageMultiplierByTag(const FGameplayTag& MultiplierTag, const int32 InLevel) const;
	
	
};
