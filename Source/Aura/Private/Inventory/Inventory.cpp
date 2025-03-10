// Copyright Jonathan Cote


#include "Inventory/Inventory.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
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
		ManageEquippedItems(&EmptyItem);
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
	if (Inventory[SlotID] == FRpgItemInfo()) return;
	if (Inventory[SlotID].ItemTypeEnum != EItemType::Equipment) return;
	
	for (auto& Item : Inventory)
	{
		if (Item.ItemType == EquipSlotTag && Item.bEquipped)
		{
			Item.bEquipped = false;
			OnItemInfoChangedDelegate.Broadcast(Item);
		}
	}
	
	Inventory[SlotID].bEquipped = true;
	ManageEquippedItems(&Inventory[SlotID]);
	
	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);
}

void UInventory::UnequipItem(const int32 SlotID, FGameplayTag EquipSlotTag)
{
	if (SlotID < 0) return;
	if (Inventory[SlotID].ItemTypeEnum != EItemType::Equipment) return;

	FRpgItemInfo EmptyItem = FRpgItemInfo();
	EmptyItem.ItemType = Inventory[SlotID].ItemType;
	EmptyItem.bEquipped = Inventory[SlotID].bEquipped;
	OnItemInfoChangedDelegate.Broadcast(EmptyItem);
	
	ManageEquippedItems(&EmptyItem);
	
	Inventory[SlotID].bEquipped = false;

	OnItemInfoChangedDelegate.Broadcast(Inventory[SlotID]);
}

void UInventory::ManageEquippedItems(const FRpgItemInfo* EquipItemInfo)
{
	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Weapon))
	{
		EquippedItems.WeaponEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.WeaponEffectHandle);
		EquippedItems.Weapon = *EquipItemInfo;
		EquippedItems.WeaponEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Weapon.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Helmet))
	{
		EquippedItems.HelmetEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.HelmetEffectHandle);
		EquippedItems.Helmet = *EquipItemInfo;
		EquippedItems.HelmetEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Helmet.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Chest))
	{
		EquippedItems.ChestEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.ChestEffectHandle);
		EquippedItems.Chest = *EquipItemInfo;
		EquippedItems.ChestEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Chest.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Legs))
	{
		EquippedItems.LegsEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.LegsEffectHandle);
		EquippedItems.Legs = *EquipItemInfo;
		EquippedItems.LegsEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Legs.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Gloves))
	{
		EquippedItems.GlovesEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.GlovesEffectHandle);
		EquippedItems.Gloves = *EquipItemInfo;
		EquippedItems.GlovesEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Gloves.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Boots))
	{
		EquippedItems.BootsEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.BootsEffectHandle);
		EquippedItems.Boots = *EquipItemInfo;
		EquippedItems.BootsEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Boots.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Necklace))
	{
		EquippedItems.NecklaceEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.NecklaceEffectHandle);
		EquippedItems.Necklace = *EquipItemInfo;
		EquippedItems.NecklaceEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Necklace.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Belt))
	{
		EquippedItems.BeltEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.BeltEffectHandle);
		EquippedItems.Belt = *EquipItemInfo;
		EquippedItems.BeltEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Belt.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else if (EquipItemInfo->ItemType.MatchesTagExact(GameplayTags.Equipment_Ring))
	{
		EquippedItems.RingEffectHandle = URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(GetOwner(), EquippedItems.RingEffectHandle);
		EquippedItems.Ring = *EquipItemInfo;
		EquippedItems.RingEffectHandle = URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(GetOwner(), EquippedItems.Ring.AttributeModifiers);
		
		UE_LOG(LogRpg, Log, TEXT("Equipping: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
	else
	{
		UE_LOG(LogRpg, Log, TEXT("Failed to find Equipment slot: [%s]"), *EquipItemInfo->ItemType.ToString());
	}
}


void UInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


