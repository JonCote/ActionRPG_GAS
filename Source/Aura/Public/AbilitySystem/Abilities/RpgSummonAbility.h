// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "RpgSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URpgSummonAbility : public URpgGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Summoning")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Ability|Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Summoning")
	float SpawnSpread = 90.f;
	
};
