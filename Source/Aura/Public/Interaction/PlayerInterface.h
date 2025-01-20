// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(const int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(const int32 InLevels);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(const int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(const int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(const int32 InXP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMagicCircleLocation(FHitResult HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag, UWorld* World);
	
};
