// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInfo.h"
#include "UI/WidgetController/RpgWidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChangedSignature, const TArray<FRpgItemInfo>&, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentSlotChangedSignature, const FGameplayTag, SlotTag, const FRpgItemInfo, ItemInfo);

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

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryChangedSignature InventoryChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FEquipmentSlotChangedSignature EquipmentSlotChangedDelegate;
	
	void OnInventoryChanged(const TArray<FRpgItemInfo>& Inventory);

	void OnEquipmentSlotChanged(const FGameplayTag InSlotTag, const FRpgItemInfo& InItemInfo);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SlotChanged(const int32 OldSlot, const int32 NewSlot);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void EquipItem(const int32 SlotID, const FGameplayTag EquipSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UnequipItem(const int32 SlotID, const FGameplayTag EquipSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveFromInventory(const int32 Slot);
};

