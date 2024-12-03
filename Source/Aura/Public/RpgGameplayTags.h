// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * RpgGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FRpgGameplayTags
{
public:
	static const FRpgGameplayTags& Get() { return GameplayTags; }
	static void InitNativeGameplayTags();
	
protected:

private:
	static FRpgGameplayTags GameplayTags;
	
};
