// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RpgGameplayTags.h"
#include "Player/RpgPlayerState.h"
#include "UI/WidgetController/RpgWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature,
												bool, bSpendPointsButtonEnabled,
												bool, bEquipButtonEnabled,
												FString, DescriptionString,
												FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityTypeTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public URpgWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|SpellPoints")
	FOnPlayerStatChangedSignature OnPlayerSpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipSelectionDelegate;

	UFUNCTION(BlueprintCallable, Category="GAS|SpellPoints")
	void SpendPointsButtonPressed();
	
	UFUNCTION(BlueprintCallable, Category="GAS|SpellPoints")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	UFUNCTION(BlueprintCallable)
	void EquippableAbilitySelected();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityTypeTag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag);

	UFUNCTION(BlueprintCallable)
	bool GetIsWaitForEquipSelection() const;
	
private:

	FSelectedAbility SelectedAbility = { FRpgGameplayTags::Get().Abilities_None, FRpgGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bEnableSpendPointsButton, bool& bEnableEquipButton);
	void BroadcastSpellGlobeSelected();

	bool bWaitForEquipSelection = false;
	
};

