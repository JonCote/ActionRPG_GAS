// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemInfo.h"
#include "UI/WidgetController/RpgWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UInventory;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemInfoSignature, const FRpgItemInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxInventorySlotsChangedSignature, int32, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UInventoryWidgetController : public URpgWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="Inventory|Messages")
	FItemInfoSignature ItemInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory|MaxSlots")
	FOnMaxInventorySlotsChangedSignature OnMaxInventorySlotsChangedDelegate;

	UPROPERTY(BlueprintReadWrite, Category="Inventory|MaxSlots")
	int32 CurrentInventorySlots = 0;

	void BroadcastItemInfo();
	
	void OnItemChanged(const FRpgItemInfo& ItemInfo) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SlotChanged(const int32 OldSlot, const int32 NewSlot);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void EquipItem(const int32 SlotID, const FGameplayTag EquipSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UnequipItem(const int32 SlotID, const FGameplayTag EquipSlotTag);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveFromInventory(const int32 Slot);

	

private:

	UInventory* GetInventoryComponent();
	
};


