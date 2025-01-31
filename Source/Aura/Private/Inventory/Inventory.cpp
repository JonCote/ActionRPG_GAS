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

	for (auto& InvSlot : Inventory)
	{
		if (InvSlot == FRpgItemInfo())
		{
			InvSlot = NewItem;
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
	Inventory[NewSlotID] = TempItem;

	OnInventoryChangedDelegate.Broadcast(Inventory);
}

TArray<FRpgItemInfo> UInventory::GetInventory()
{
	return Inventory;
}

void UInventory::EquipItem(const FString& ItemName, FGameplayTag EquipSlotTag)
{
	const FRpgItemInfo NewItem = ItemInfo->FindItemInfoByName(ItemName);

	EquippedItems.Emplace(EquipSlotTag) = NewItem;

	OnEquipmentChangedDelegate.Broadcast(EquipSlotTag, NewItem);
	
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


