// Copyright Jonathan Cote


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/RpgLogChannels.h"

FRpgAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FRpgAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTag(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogRpg, Error, TEXT("Attribute not found for given AttributeTag [%s] on AttributeInfo [%s]"),
			*AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FRpgAttributeInfo();
}
