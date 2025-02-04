// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryItemInfo.generated.h"


class ALootableItem;

USTRUCT(BlueprintType)
struct FRpgItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString ItemName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ItemType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ALootableItem> ItemClass;

	UPROPERTY(BlueprintReadOnly)
	bool bEquipped = false;

	UPROPERTY(BlueprintReadOnly)
	int32 InventorySlotID = -1;
	
};
inline bool operator==(const FRpgItemInfo& Left, const FRpgItemInfo& Right)
{
	return Left.ItemName == Right.ItemName;
}


/**
 * 
 */
UCLASS()
class AURA_API UInventoryItemInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	FRpgItemInfo FindItemInfoByName(const FString& ItemName, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Item Information")
	TArray<FRpgItemInfo> ItemInformation;
	
};
