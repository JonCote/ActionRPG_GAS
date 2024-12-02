// Copyright Jonathan Cote


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	const URpgAttributeSet* RpgAttributeSet = CastChecked<URpgAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(RpgAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RpgAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(RpgAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(RpgAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const URpgAttributeSet* RpgAttributeSet = CastChecked<URpgAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	RpgAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	RpgAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RpgAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RpgAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	Cast<URpgAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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

