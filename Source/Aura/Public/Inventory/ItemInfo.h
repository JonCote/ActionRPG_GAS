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


USTRUCT(BlueprintType)
struct FRpgEquippableItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag EquipmentType = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat AttackPower = 0.f;
};

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
	FRpgEquippableItemInfo EquippableItemInfo = FRpgEquippableItemInfo();
	
	UPROPERTY(BlueprintReadOnly)
	bool bEquipped = false;

	UPROPERTY(BlueprintReadOnly)
	int32 InventorySlotID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayAttribute, float> AttributeModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARpgCharacter* OwningCharacter = nullptr;

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
