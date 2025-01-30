// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInfo.h"
#include "Components/ActorComponent.h"
#include "Interaction/InventoryInterface.h"
#include "Inventory.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UInventory : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventory();

	/* Inventory Interface */
	virtual FOnInventoryChanged& GetOnInventoryChangedDelegate() override;
	
	/* End Inventory Interface */

	FOnInventoryChanged OnInventoryChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Item Info")
	TObjectPtr<UInventoryItemInfo> ItemInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlots = 4;

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
	
protected:
	virtual void BeginPlay() override;

	
private:




		
};
