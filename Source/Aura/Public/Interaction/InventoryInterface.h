// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInfo.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FRpgItemInfo>& /*Inventory*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, const FGameplayTag /*EquipSlotType*/, const FRpgItemInfo& /*Inventory*/);



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

	virtual FOnInventoryChanged& GetOnInventoryChangedDelegate() = 0;
	virtual FOnEquipmentChanged& GetOnEquipmentChangedDelegate() = 0;
	
};
