// Copyright Jonathan Cote


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/RpgPlayerState.h"

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadcastAbilityInfo();
	OnPlayerSpellPointsChangedDelegate.Broadcast(GetRpgPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetRpgAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewAbilityLevel)
		{
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.StatusTag = StatusTag;
				BroadcastSpellGlobeSelected();
			}
			if (AbilityInfo)
			{
				FRpgAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetRpgAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	GetRpgAbilitySystemComponent()->AbilityDescriptionDelegate.AddUObject(this, &USpellMenuWidgetController::OnDescriptionAcquired);

	GetRpgPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerSpellPointsChangedDelegate.Broadcast(NewValue);

			CurrentSpellPoints = NewValue;
			BroadcastSpellGlobeSelected();
		});
}

void USpellMenuWidgetController::SpendPointsButtonPressed()
{
	if (GetRpgAbilitySystemComponent())
	{
		GetRpgAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
		GetRpgAbilitySystemComponent()->ServerGetDescriptionsByAbilityTag(SelectedAbility.AbilityTag);
	}
	
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	const FGameplayAbilitySpec* AbilitySpec = GetRpgAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	CurrentSpellPoints = GetRpgPlayerState()->GetSpellPoints();

	if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
	{
		// User is Deselecting a Selected Ability
		SelectedAbility.AbilityTag = GameplayTags.Abilities_None;
		SelectedAbility.StatusTag = GameplayTags.Abilities_Status_Locked;
		SelectedAbility.InputTag = FGameplayTag();
		SelectedAbility.DescriptionString = FString();
		SelectedAbility.NextLevelDescriptionString = FString();
	}
	else
	{
		FGameplayTag AbilityStatus;
		
		const bool bTagValid = AbilityTag.IsValid() && (AbilityTag != FRpgGameplayTags::Get().Abilities_None);
		const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
		const bool bSpecValid = (AbilitySpec != nullptr);
		if (!bSpecValid || bTagNone || !bTagValid)
		{
			AbilityStatus = GameplayTags.Abilities_Status_Locked;
		}
		else
		{
			AbilityStatus = GetRpgAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
			
		}

		
		GetRpgAbilitySystemComponent()->ServerGetDescriptionsByAbilityTag(AbilityTag);
		
		
		SelectedAbility.AbilityTag = AbilityTag;
		SelectedAbility.StatusTag = AbilityStatus;
	}

	BroadcastSpellGlobeSelected();
}

//TODO: Prob a better way to do this! (i.e using destructor or something)
void USpellMenuWidgetController::CloseMenu()
{
	SelectedAbility.AbilityTag = FRpgGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FRpgGameplayTags::Get().Abilities_Status_Locked;
	SelectedAbility.InputTag = FGameplayTag();
	SelectedAbility.DescriptionString = FString();
	SelectedAbility.NextLevelDescriptionString = FString();
	BroadcastSpellGlobeSelected();
}

void USpellMenuWidgetController::EquippableAbilitySelected()
{
	if (SelectedAbility.AbilityTag.MatchesTagExact(FRpgGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedAbility.InputTag = GetRpgAbilitySystemComponent()->GetSlotTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityTypeTag)
{
	if (!bWaitForEquipSelection) { return; }
	// Check selected ability against the slot's ability type. (don't equip an active spell in a passive slot and vice versa)
	const FGameplayTag& SelectedAbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityTypeTag;
	if (!SelectedAbilityTypeTag.MatchesTagExact(AbilityTypeTag)) { return; }
	
	GetRpgAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
		
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();

	FRpgAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.InputTag = PrevSlotTag;

	// Broadcast empty info if PrevSlotTag is a valid SlotTag. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FRpgAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	SelectedAbility.InputTag = Info.InputTag;
	SelectedAbility.StatusTag = Info.AbilityTag;
	
	AbilityInfoDelegate.Broadcast(Info);
	
}

void USpellMenuWidgetController::OnDescriptionAcquired(const FGameplayTag& AbilityTag, const FString& Description,
	const FString& NextLevelDescription)
{
	SelectedAbility.DescriptionString = Description;
	SelectedAbility.NextLevelDescriptionString = NextLevelDescription;
	BroadcastSpellGlobeSelected();
}

bool USpellMenuWidgetController::GetIsWaitForEquipSelection() const
{
	return bWaitForEquipSelection;
}

void USpellMenuWidgetController::BroadcastSpellGlobeSelected()
{
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
	
	if (bEnableEquip)
	{
		bWaitForEquipSelection = true;
		// Start Animations TODO: Add animations
		WaitForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityTypeTag);
		EquippableAbilitySelected();
	}
	else
	{
		bWaitForEquipSelection = false;
		// Stop Animations TODO: Add animations
		StopWaitingForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityTypeTag);
	}
	
	OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, SelectedAbility.DescriptionString, SelectedAbility.NextLevelDescriptionString);
}


void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints,
                                                     bool& bEnableSpendPointsButton, bool& bEnableEquipButton)
{
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
	bEnableSpendPointsButton = false;
	bEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bEnableSpendPointsButton = true;
		}
	}
}



