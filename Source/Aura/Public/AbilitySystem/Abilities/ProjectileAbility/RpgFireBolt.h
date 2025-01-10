// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RpgProjectileSpell.h"
#include "RpgFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URpgFireBolt : public URpgProjectileSpell
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(const FString& AbilityName, const int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FVector& SpawnLocation, AActor* HomingTarget, const float PitchOverride=0.f);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	FScalableFloat TargetLocationHitRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	int32 MaxNumberOfProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	bool bLaunchHomingProjectiles = true;
};
