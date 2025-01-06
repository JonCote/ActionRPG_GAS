// Copyright Jonathan Cote


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/RpgPlayerState.h"

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair : GetRpgAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	OnPlayerAttributePointsChangedDelegate.Broadcast(GetRpgPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetRpgAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	
	GetRpgPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerAttributePointsChangedDelegate.Broadcast(NewValue);
		}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetRpgAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FRpgAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


