// Copyright Jonathan Cote


#include "UI/WidgetController/OverlayWidgetController.h"

#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/RpgPlayerState.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	OnHealthChanged.Broadcast(GetRpgAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetRpgAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetRpgAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetRpgAttributeSet()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetRpgPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetRpgPlayerState()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetRpgAttributeSet()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetRpgAttributeSet()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetRpgAttributeSet()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetRpgAttributeSet()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	
	if (GetRpgAbilitySystemComponent())
	{
		GetRpgAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetRpgAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetRpgAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	
		GetRpgAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
	
}


void UOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetRpgPlayerState()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out RpgPlayerState Blueprint"));
	
	const int32 Level = LevelUpInfo->FindLevelForGivenXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 DeltaXP = NewXP - PrevLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(DeltaXP) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag) const
{
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();

	//TODO: If swapping a equipped ability to a already populated slot, make LastSlotInfo the Ability Info the Ability in the targeted slot
	FRpgAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.InputTag = PrevSlotTag;
	// Broadcast empty info if PrevSlotTag is a valid SlotTag. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FRpgAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	
	AbilityInfoDelegate.Broadcast(Info);
}

