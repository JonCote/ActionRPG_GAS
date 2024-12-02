// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RpgAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API URpgAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

protected:

	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

};
