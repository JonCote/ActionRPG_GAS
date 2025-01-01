// Copyright Jonathan Cote


#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/RpgLogChannels.h"

FRpgAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag AbilityTag, bool bLogNotFound) const
{
	for (const FRpgAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTag(AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogRpg, Error, TEXT("Ability not found for given AbilityTag [%s] on AbilityInfo [%s]"),
			*AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FRpgAbilityInfo();
}
