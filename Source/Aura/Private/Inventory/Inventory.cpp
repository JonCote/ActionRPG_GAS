// Copyright Jonathan Cote


#include "Inventory/Inventory.h"

#include "RpgGameplayTags.h"
#include "Actor/LootableItem.h"
#include "Aura/RpgLogChannels.h"


UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	for (int32 i = 0; i < InventorySlots; i++)
	{
		Inventory.Add(FRpgItemInfo());
		Inventory[i].InventorySlotID = i;
	}
}

FOnItemInfoChanged& UInventory::GetOnItemInfoChangedDelegate()
{
	return OnItemInfoChangedDelegate;
}

FOnInventorySlotsChanged& UInventory::GetOnInventorySlotsChangedDelegate()
{
	return OnInventorySlotsChangedDelegate;
}

void UInventory::ForEachItem(const FForEachItem& Delegate)
{
	for (const FRpgItemInfo& Item : Inventory)
	{
		if (!Delegate.ExecuteIfBound(Item))
		{
			UE_LOG(LogRpg, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FRpgItemInfo UInventory::GetItemInfoInSlot(const int32 SlotID)
{
	return Inventory[SlotID];
}

void UInventory::RemoveItemInfoInSlot(const int32 SlotID)
{
	if (!IsValid(Inventory[SlotID].ItemClass.Get())) return;
	
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 150.f;
	GetWorld()->SpawnActor(Inventory[SlotID].ItemClass, &SpawnLocation);
	
	if (Inventory[SlotID].bEquipped)
	{
		FRpgItemInfo EmptyItem = FRpgItemInfo();
		EmptyItem.ItemType = Inventory[SlotID].ItemType;
		EmptyItem.bEquipped = Inventory[SlotID].bEquipped;
		OnItemInfoChangedDelegate.Broadcast(EmptyItem);
	}
	
	Inventory[SlotID] = FRpgItemInfo();
	Inventory[SlotID].InventorySlotID = SlotID;

	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);
}

void UInventory::SetItemInfoInSlot(const int32 SlotID, const FRpgItemInfo Info)
{
	Inventory[SlotID] = Info;
}

bool UInventory::AddItemToInventory(const FString& ItemName)
{
	const FRpgItemInfo NewItem = ItemInfo->FindItemInfoByName(ItemName);

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] == FRpgItemInfo())
		{
			Inventory[i] = NewItem;
			Inventory[i].InventorySlotID = i;
			OnItemInfoChangedDelegate.Broadcast(Inventory[i]);
			
			return true;
		}
	}

	UE_LOG(LogRpg, Warning, TEXT("Inventory full [%s] can not be looted"), *NewItem.ItemName);
	return false;
}

void UInventory::SwapItemInfoInSlots(const int32 SlotID, const int32 NewSlotID)
{
	if (SlotID > InventorySlots || NewSlotID > InventorySlots) return;
	
	const FRpgItemInfo TempItem = Inventory[SlotID];
	Inventory[SlotID] = Inventory[NewSlotID];
	Inventory[SlotID].InventorySlotID = SlotID;
	Inventory[NewSlotID] = TempItem;
	Inventory[NewSlotID].InventorySlotID = NewSlotID;

	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);
	OnItemInfoChangedDelegate.Broadcast(Inventory[NewSlotID]);
	
}

TArray<FRpgItemInfo> UInventory::GetInventory()
{
	return Inventory;
}

TArray<FRpgItemInfo> UInventory::GetEquipped()
{
	TArray<FRpgItemInfo> OutItems;
	for (auto& Item : Inventory)
	{
		if (Item.bEquipped)
		{
			OutItems.Add(Item);
		}
	}
	return OutItems;
}

int32 UInventory::GetInventorySlotCount() const
{
	return InventorySlots;
}

void UInventory::SetInventorySlotCount(const int32 SlotCount)
{
	InventorySlots = SlotCount;
}

void UInventory::EquipItem(const int32 SlotID, FGameplayTag EquipSlotTag)
{
	
	for (auto& Item : Inventory)
	{
		if (Item.ItemType == EquipSlotTag && Item.bEquipped)
		{
			Item.bEquipped = false;
			OnItemInfoChangedDelegate.Broadcast(Item);
		}
	}
	
	Inventory[SlotID].bEquipped = true;
	
	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);

	
	
}

void UInventory::UnequipItem(const int32 SlotID, FGameplayTag EquipSlotTag)
{
	if (SlotID < 0) return;

	FRpgItemInfo EmptyItem = FRpgItemInfo();
	EmptyItem.ItemType = Inventory[SlotID].ItemType;
	EmptyItem.bEquipped = Inventory[SlotID].bEquipped;
	OnItemInfoChangedDelegate.Broadcast(EmptyItem);
	
	Inventory[SlotID].bEquipped = false;

	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);
}



void UInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


