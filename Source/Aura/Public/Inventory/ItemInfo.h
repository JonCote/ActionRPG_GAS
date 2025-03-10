// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Character/RpgCharacter.h"
#include "Engine/DataAsset.h"
#include "ItemInfo.generated.h"

class ALootableItem;


UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Equipment = 1	UMETA(DisplayName = "Equipment"),
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType ItemTypeEnum = EItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemTypeEnum == EItemType::Equipment", EditConditionHides))
	FGameplayTag EquipmentTypeTag = FGameplayTag();
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemTypeEnum == EItemType::Equipment", EditConditionHides))
	TMap<FGameplayAttribute, float> AttributeModifiers = TMap<FGameplayAttribute, float>();

	
	UPROPERTY(BlueprintReadOnly)
	ARpgCharacter* OwningCharacter = nullptr;

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
class AURA_API UItemInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	FRpgItemInfo FindItemInfoByName(const FString& ItemName, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Item Information")
	TArray<FRpgItemInfo> ItemInformation;
	
};
