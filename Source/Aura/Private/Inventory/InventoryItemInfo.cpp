// Copyright Jonathan Cote


#include "Inventory/InventoryItemInfo.h"

#include "Aura/RpgLogChannels.h"

FRpgItemInfo UInventoryItemInfo::FindItemInfoByName(const FString& ItemName, bool bLogNotFound) const
{
	for (const FRpgItemInfo& Item : ItemInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogRpg, Error, TEXT("Item not found for given ItemName [%s] on InventoryItemInfo [%s]"),
			*ItemName, *GetNameSafe(this));
	}
	
	return FRpgItemInfo();
}
