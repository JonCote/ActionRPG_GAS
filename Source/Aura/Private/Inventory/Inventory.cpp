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
	}

	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Chest, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Helmet, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Belt, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Boots, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Gloves, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Legs, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Necklace, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Ring, FRpgItemInfo());
	EquippedItems.Add(FRpgGameplayTags::Get().Equipment_Weapon, FRpgItemInfo());
}

FOnInventoryChanged& UInventory::GetOnInventoryChangedDelegate()
{
	return OnInventoryChangedDelegate;
}

FOnEquipmentChanged& UInventory::GetOnEquipmentChangedDelegate()
{
	return OnEquipmentChangedDelegate;
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
		EquippedItems.Emplace(Inventory[SlotID].ItemType) = FRpgItemInfo();
		OnEquipmentChangedDelegate.Broadcast(Inventory[SlotID].ItemType, FRpgItemInfo());
	}
	
	Inventory[SlotID].bEquipped = false;
	Inventory[SlotID] = FRpgItemInfo();
	OnInventoryChangedDelegate.Broadcast(Inventory);
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
			
			OnInventoryChangedDelegate.Broadcast(Inventory);
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

	OnInventoryChangedDelegate.Broadcast(Inventory);
}

TArray<FRpgItemInfo> UInventory::GetInventory()
{
	return Inventory;
}

TArray<FRpgItemInfo> UInventory::GetEquipped()
{
	TArray<FRpgItemInfo> OutItems;
	for (auto& Item : EquippedItems)
	{
		OutItems.Add(Item.Value);
	}
	return OutItems;
}

void UInventory::EquipItem(const int32 SlotID, FGameplayTag EquipSlotTag)
{
	if (!EquippedItems.Find(EquipSlotTag)) return;

	if (EquippedItems.Find(EquipSlotTag)->bEquipped)
	{
		Inventory[EquippedItems.Find(EquipSlotTag)->InventorySlotID].bEquipped = false;
	}
	
	Inventory[SlotID].bEquipped = true;
	
	EquippedItems.Emplace(EquipSlotTag) = Inventory[SlotID];

	OnEquipmentChangedDelegate.Broadcast(EquipSlotTag, Inventory[SlotID]);
	
}

void UInventory::UnequipItem(const int32 SlotID, FGameplayTag EquipSlotTag)
{
	if (!EquippedItems.Find(EquipSlotTag)) return;

	Inventory[SlotID].bEquipped = false;
	EquippedItems.Emplace(EquipSlotTag) = FRpgItemInfo();

	OnEquipmentChangedDelegate.Broadcast(EquipSlotTag, FRpgItemInfo());
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


