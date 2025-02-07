// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemInfo.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemInfoChanged, const FRpgItemInfo& /*ItemInfo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotsChanged, int32);
DECLARE_DELEGATE_OneParam(FForEachItem, const FRpgItemInfo&);




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FOnItemInfoChanged& GetOnItemInfoChangedDelegate() = 0;
	virtual FOnInventorySlotsChanged& GetOnInventorySlotsChangedDelegate() = 0;
	virtual void ForEachItem(const FForEachItem& Delegate) = 0;
	
};
