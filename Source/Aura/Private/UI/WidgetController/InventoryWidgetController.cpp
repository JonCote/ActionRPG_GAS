// Copyright Jonathan Cote


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Character/RpgCharacter.h"
#include "Inventory/Inventory.h"
#include "Player/RpgPlayerState.h"

void UInventoryWidgetController::BroadCastInitialValues()
{
	
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->OnInventoryChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnInventoryChanged);
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->OnEquipmentChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnEquipmentSlotChanged);
}

void UInventoryWidgetController::OnInventoryChanged(const TArray<FRpgItemInfo>& Inventory)
{
	InventoryChangedDelegate.Broadcast(Inventory);
}

void UInventoryWidgetController::OnEquipmentSlotChanged(const FGameplayTag InSlotTag, const FRpgItemInfo& InItemInfo)
{
	EquipmentSlotChangedDelegate.Broadcast(InSlotTag, InItemInfo);
}

void UInventoryWidgetController::SlotChanged(const int32 OldSlot, const int32 NewSlot)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->SwapItemInfoInSlots(OldSlot, NewSlot);
}

void UInventoryWidgetController::EquipItem(const int32 SlotID, const FGameplayTag EquipSlotTag)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->EquipItem(SlotID, EquipSlotTag);
}

void UInventoryWidgetController::UnequipItem(const int32 SlotID, const FGameplayTag EquipSlotTag)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->UnequipItem(SlotID, EquipSlotTag);
}

void UInventoryWidgetController::RemoveFromInventory(const int32 Slot)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->RemoveItemInfoInSlot(Slot);
}


