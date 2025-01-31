// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInfo.h"
#include "Components/ActorComponent.h"
#include "Interaction/InventoryInterface.h"
#include "Inventory.generated.h"


struct FGameplayTag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UInventory : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventory();

	/* Inventory Interface */
	virtual FOnInventoryChanged& GetOnInventoryChangedDelegate() override;
	virtual FOnEquipmentChanged& GetOnEquipmentChangedDelegate() override;
	
	/* End Inventory Interface */

	FOnInventoryChanged OnInventoryChangedDelegate;
	FOnEquipmentChanged OnEquipmentChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Item Info")
	TObjectPtr<UInventoryItemInfo> ItemInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlots = 12;

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

	UPROPERTY(BlueprintReadOnly)
	TArray<FRpgItemInfo> Inventory;


	/* Equipment slots */

	void EquipItem(const FString& ItemName, FGameplayTag EquipSlotTag);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory|Equipped")
	TMap<FGameplayTag, FRpgItemInfo> EquippedItems;

	
protected:
	virtual void BeginPlay() override;

	
	
private:




		
};
