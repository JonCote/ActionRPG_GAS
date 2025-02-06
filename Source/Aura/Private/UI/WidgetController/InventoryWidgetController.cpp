// Copyright Jonathan Cote


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Character/RpgCharacter.h"
#include "Inventory/Inventory.h"
#include "Player/RpgPlayerState.h"

void UInventoryWidgetController::BroadCastInitialValues()
{
	OnMaxInventorySlotsChangedDelegate.Broadcast(GetInventoryComponent()->GetInventorySlotCount());
	BroadcastItemInfo();
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	GetInventoryComponent()->OnItemInfoChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnItemChanged);
	GetInventoryComponent()->OnInventorySlotsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnMaxInventorySlotsChangedDelegate.Broadcast(NewValue);
			CurrentInventorySlots = NewValue;
		});
}

void UInventoryWidgetController::BroadcastItemInfo()
{
	FForEachItem BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FRpgItemInfo& ItemInfo)
	{
		ItemInfoDelegate.Broadcast(ItemInfo);
	});

	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->ForEachItem(BroadcastDelegate);
}

void UInventoryWidgetController::OnItemChanged(const FRpgItemInfo& ItemInfo) const
{
	ItemInfoDelegate.Broadcast(ItemInfo);
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



UInventory* UInventoryWidgetController::GetInventoryComponent()
{
	return Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory;
}


