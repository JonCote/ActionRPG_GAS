// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "ItemInfo.h"
#include "Components/ActorComponent.h"
#include "Interaction/InventoryInterface.h"
#include "Inventory.generated.h"

struct FGameplayTag;


USTRUCT(BlueprintType)
struct FEquippedItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Helmet = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle HelmetEffectHandle;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Chest = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle ChestEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Legs = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle LegsEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Gloves = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle GlovesEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Boots = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle BootsEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Weapon = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle WeaponEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Necklace = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle NecklaceEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Belt = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle BeltEffectHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRpgItemInfo Ring = FRpgItemInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle RingEffectHandle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UInventory : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventory();

	/* Inventory Interface */
	virtual FOnItemInfoChanged& GetOnItemInfoChangedDelegate() override;
	virtual FOnInventorySlotsChanged& GetOnInventorySlotsChangedDelegate() override;
	virtual void ForEachItem(const FForEachItem& Delegate) override;
	
	/* End Inventory Interface */

	FOnItemInfoChanged OnItemInfoChangedDelegate;
	FOnInventorySlotsChanged OnInventorySlotsChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Item Info")
	TObjectPtr<UItemInfo> ItemInfo;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FRpgItemInfo GetItemInfoInSlot(const int32 SlotID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemInfoInSlot(const int32 SlotID);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemInfoInSlot(const int32 SlotID, const FRpgItemInfo Info);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(const FString& ItemName);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItemInfoInSlots(const int32 SlotID, const int32 NewSlotID);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FRpgItemInfo> GetInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FRpgItemInfo> GetEquipped();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySlotCount() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventorySlotCount(const int32 SlotCount);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FRpgItemInfo> Inventory;

	UPROPERTY(BlueprintReadOnly)
	FEquippedItems EquippedItems;
	
	/* Equipment slots */
	void EquipItem(const int32 SlotID, FGameplayTag EquipSlotTag);
	void UnequipItem(const int32 SlotID, FGameplayTag EquipSlotTag);
	bool ManageEquippedItems(const FRpgItemInfo* ItemInfo);
	
protected:
	virtual void BeginPlay() override;

	
	
private:

	int32 InventorySlots = 48;




		
};
