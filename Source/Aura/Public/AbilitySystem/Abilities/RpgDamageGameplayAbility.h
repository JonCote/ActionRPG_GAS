// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "RpgAbilityTypes.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "RpgDamageGameplayAbility.generated.h"


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

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	float GetBaseDamage(const int32 InLevel) const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	FDamageInfo DamageInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TArray<FDebuffInfo> DebuffInfo;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
	
	float GetDamageMultiplierByTag(const FGameplayTag& MultiplierTag, const int32 InLevel) const;
	
	
};
