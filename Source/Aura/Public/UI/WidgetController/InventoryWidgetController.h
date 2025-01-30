// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInfo.h"
#include "UI/WidgetController/RpgWidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChangedSignature, const TArray<FRpgItemInfo>&, Inventory);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UInventoryWidgetController : public URpgWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FInventoryChangedSignature InventoryChangedDelegate;
	
	void OnInventoryChanged(const TArray<FRpgItemInfo>& Inventory);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SlotChanged(const int32 OldSlot, const int32 NewSlot);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveFromInventory(const int32 Slot);
};
