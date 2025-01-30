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
}

void UInventoryWidgetController::OnInventoryChanged(const TArray<FRpgItemInfo>& Inventory)
{
	InventoryChangedDelegate.Broadcast(Inventory);
}

void UInventoryWidgetController::SlotChanged(const int32 OldSlot, const int32 NewSlot)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->SwapItemInfoInSlots(OldSlot, NewSlot);
}

void UInventoryWidgetController::RemoveFromInventory(const int32 Slot)
{
	Cast<ARpgCharacter>(GetRpgPlayerState()->GetPawn())->Inventory->RemoveItemInfoInSlot(Slot);
}


