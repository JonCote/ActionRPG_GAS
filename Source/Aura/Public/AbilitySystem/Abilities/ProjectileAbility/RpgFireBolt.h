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
};
