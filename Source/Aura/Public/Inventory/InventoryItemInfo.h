// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemInfo.generated.h"


USTRUCT()
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString ItemName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
};


/**
 * 
 */
UCLASS()
class AURA_API UInventoryItemInfo : public UDataAsset
{
	GENERATED_BODY()

	FInventoryItemInfo FindItemInfoByName(const FString& ItemName, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Item Information")
	TArray<FInventoryItemInfo> ItemInformation;
	
};
