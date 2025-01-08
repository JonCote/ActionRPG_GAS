// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RpgGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API URpgGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(const FString& AbilityName, const int32 Level);
	static FString GetLockedDescription(const FString& AbilityName, const int32 Level);

protected:
	
	float GetManaCost(const int32 InLevel = 1.f) const;
	float GetCooldown(const int32 InLevel = 1.f) const;
};
