// Copyright Jonathan Cote


#include "Input/RpgInputConfig.h"

const UInputAction* URpgInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FRpgInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find AbilityInputAction for InputTag [%s], on InputConfig [%s]"),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
